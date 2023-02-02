#include "slpch.h"
#include "Model.h"
#include "Soul/Renderer/Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Soul
{
	Model::Model(std::string path)
	{
		LoadModel(path);
	}

	void Model::Draw(const Ref<Shader>& shader, const glm::mat4& transform)
	{
		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
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

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

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

		return Mesh(vertices, indices);
	}
}