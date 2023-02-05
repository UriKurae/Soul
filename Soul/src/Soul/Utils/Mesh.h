#pragma once

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glm/glm.hpp>
#include <Soul/Renderer/Shader.h>
#include <Soul/Renderer/Texture.h>

namespace Soul
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& vert, std::vector<uint32_t>& ind, std::vector<Ref<Texture2D>>& texts);
		~Mesh();

		Ref<VertexArray> GetVertexArrayObject() { return vao; }
		std::vector<Ref<Texture2D>> GetTextures() { return textures; }
		void BindTextures();

	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Ref<Texture2D>> textures;

		Ref<VertexArray> vao;
		Ref<VertexBuffer> vbo;
		Ref<IndexBuffer> ebo;
		
		ShaderLibrary shaderLib;
		void setupMesh();
	};
}
