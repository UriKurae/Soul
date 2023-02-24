#pragma once

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glm/glm.hpp>
#include <Soul/Renderer/Shader.h>
#include <Soul/Renderer/Texture.h>

namespace Soul
{
	struct Triangle
	{
		glm::vec3 a;
		glm::vec3 b;
		glm::vec3 c;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& vert, std::vector<uint32_t>& ind, std::vector<Ref<Texture2D>>& texts);
		~Mesh();

		Ref<VertexArray> GetVertexArrayObject() { return vao; }
		std::vector<Ref<Texture2D>> GetTextures() { return textures; }
		void BindTextures();

		std::vector<Triangle>& GetTriangles() { return meshTriangles; }
		std::vector<Vertex>& GetVertices() { return vertices; }
	private:
		void CalculateTriangles();
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Ref<Texture2D>> textures;
		std::vector<Triangle> meshTriangles;

		Ref<VertexArray> vao;
		Ref<VertexBuffer> vbo;
		Ref<IndexBuffer> ebo;
		
		ShaderLibrary shaderLib;
		void setupMesh();
	};
}
