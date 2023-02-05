#pragma once

#include "Soul/Utils/Mesh.h"
#include "Soul/Renderer/Shader.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;


namespace Soul
{
	class Model
	{
	public:
		Model(std::string path);

		void Draw(const Ref<Shader>& shader, const glm::mat4& transform);
	private:
		std::vector<Mesh> meshes;
		std::vector<Ref<Texture2D>> loadedTextures;
		std::string directory;

		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* mat, TextureType type, std::string typeName);
		
	};
}