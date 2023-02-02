#pragma once

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glm/glm.hpp>

namespace Soul
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& vert, std::vector<uint32_t>& ind);
		~Mesh();

		Ref<VertexArray> GetVertexArrayObject() { return vao; }

	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		Ref<VertexArray> vao;
		Ref<VertexBuffer> vbo;
		Ref<IndexBuffer> ebo;

		void setupMesh();
	};
}
