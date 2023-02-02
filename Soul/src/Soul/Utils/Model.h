#pragma once

#include "Soul/Utils/Mesh.h"
#include "Soul/Renderer/Shader.h"

struct aiNode;
struct aiMesh;
struct aiScene;

namespace Soul
{
	class Model
	{
	public:
		Model(std::string path);

		void Draw(const Ref<Shader>& shader, const glm::mat4& transform);
	private:
		std::vector<Mesh> meshes;
		std::string directory;

		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		
	};
}