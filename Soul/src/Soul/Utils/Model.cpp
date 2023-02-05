#include "slpch.h"
#include "Model.h"
#include "Soul/Renderer/Renderer.h"

#include <stack>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <stb_image.h>

namespace Soul
{
	static aiTextureType ToAiTextureType(TextureType type)
	{
		switch (type)
		{
		case TextureType::TEXT_DIFFUSE:
			return aiTextureType_DIFFUSE;
			break;
		case TextureType::TEXT_SPECULAR:
			return aiTextureType_SPECULAR;
			break;
		default:
			return aiTextureType_UNKNOWN;
		}
	}

	Model::Model(std::string path)
	{
		LoadModel(path);
	}

	void Model::Draw(const Ref<Shader>& shader, const glm::mat4& transform)
	{
		if (meshes.empty())
			return;

		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			meshes[i].BindTextures();
			Renderer::Submit(shader, meshes[i].GetVertexArrayObject(), transform);
		}
	}

	void Model::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			SL_CORE_ERROR("Error form assimp {0}", importer.GetErrorString());
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		SL_CORE_WARN("Starting model");
		ProcessNode(scene->mRootNode, scene);
		SL_CORE_WARN("Model sucessfully loaded");
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		std::stack<aiNode*> nodes;

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			nodes.push(node->mChildren[i]);
		}

		while (!nodes.empty())
		{
			aiNode *node = nodes.top();
			nodes.pop();

			for (unsigned int i = 0; i < node->mNumMeshes; ++i)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}

			for (int i = 0; i < node->mNumChildren; i++)
			{
				nodes.push(node->mChildren[i]);
			}
		}


		// TODO: This is the old way, recursive, not deleting yet just in case.
		/*for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene);
		}*/
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Ref<Texture2D>> texts;

		// Get all vertex info
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;

			glm::vec3 pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			glm::vec3 normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texture = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
				vertex = {pos, normal, texture};
			}
			else
			{
				vertex = { pos, normal, glm::vec2(0.0f, 0.0f)};
			}
			vertices.push_back(vertex);
		}

		// Get all indices info
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Ref<Texture2D>> diffuseMaps = LoadMaterialTextures(material, TextureType::TEXT_DIFFUSE, "u_Texture");
			texts.insert(texts.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Ref<Texture2D>> specularMaps = LoadMaterialTextures(material, TextureType::TEXT_SPECULAR, "texture_specular");
			texts.insert(texts.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, texts);
	}

	std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, TextureType type, std::string typeName)
	{
		std::vector<Ref<Texture2D>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(ToAiTextureType(type)); ++i)
		{
			aiString str;
			mat->GetTexture(ToAiTextureType(type), i, &str);

			std::string texturePath = directory + '/' + str.C_Str();

			bool skip = false;
			for (unsigned int j = 0; j < loadedTextures.size(); ++j)
			{
				if (std::strcmp(loadedTextures[j]->GetPath().c_str(), texturePath.c_str()))
				{
					textures.push_back(loadedTextures[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				Ref<Texture2D> texture = Texture2D::Create(texturePath);
				textures.push_back(texture);
				loadedTextures.push_back(texture);
			}
		}
		return textures;
	}
}