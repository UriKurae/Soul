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
		case TextureType::TEXT_NORMAL:
			return aiTextureType_NORMALS;
			break;
		case TextureType::TEXT_HEIGHT:
			return aiTextureType_HEIGHT;
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
			Renderer::Submit(shader, meshes[i].GetVertexArrayObject(), transform);
		}
	}

	void Model::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

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
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Ref<Texture2D>> texts;
		
		int totalTriangles = mesh->mNumVertices / 3;
		for (unsigned int i = 0; i < mesh->mNumVertices; i+=3)
		{
			Triangle triangle;
			triangle.a = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			triangle.b = { mesh->mVertices[i+1].x, mesh->mVertices[i+1].y, mesh->mVertices[i+1].z };
			triangle.c = { mesh->mVertices[i+2].x, mesh->mVertices[i+2].y, mesh->mVertices[i+2].z };
			modelTriangles.push_back(triangle);
		}

		// Get all vertex info
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;

			glm::vec3 pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			glm::vec3 normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			glm::vec3 tangents = {};
			glm::vec3 bitangents = {};
			if (mesh->HasTangentsAndBitangents())
			{
				tangents = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				bitangents = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texture = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
				vertex = {pos, normal, texture, tangents, bitangents };
			}
			else
			{
				vertex = { pos, normal, glm::vec2(0.0f, 0.0f), tangents, bitangents };
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

		
		// TODO: Moved textures to Material
		//if (mesh->mMaterialIndex >= 0)
		//{
		//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//	std::vector<Ref<Texture2D>> diffuseMaps = LoadMaterialTextures(material, TextureType::TEXT_DIFFUSE, "u_Texture");
		//	texts.insert(texts.end(), diffuseMaps.begin(), diffuseMaps.end());
		//	std::vector<Ref<Texture2D>> specularMaps = LoadMaterialTextures(material, TextureType::TEXT_SPECULAR, "texture_specular");
		//	texts.insert(texts.end(), specularMaps.begin(), specularMaps.end());
		//	std::vector<Ref<Texture2D>> normalMaps = LoadMaterialTextures(material, TextureType::TEXT_HEIGHT, "texture_normal");
		//	texts.insert(texts.end(), normalMaps.begin(), normalMaps.end());
		//}

		return Mesh(vertices, indices, texts);
	}


	std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, TextureType type, std::string typeName)
	{
		std::vector<Ref<Texture2D>> textures;
		unsigned int matType = mat->GetTextureCount(ToAiTextureType(type));
		for (unsigned int i = 0; i < matType; ++i)
		{
			aiString str;
			mat->GetTexture(ToAiTextureType(type), i, &str);

			std::string texturePath = directory + '/' + str.C_Str();

			bool skip = false;
			for (unsigned int j = 0; j < loadedTextures.size(); ++j)
			{
				std::string a = loadedTextures[j]->GetPath().c_str();
				std::string b = texturePath.c_str();
				
				if (std::strcmp(a.c_str(), b.c_str()) == 0)
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

	std::vector<Mesh>& Model::GetMeshes() 
	{
		return meshes;
	}

	std::vector<Triangle>& Model::GetTriangles()
	{
		return modelTriangles;
	}
}