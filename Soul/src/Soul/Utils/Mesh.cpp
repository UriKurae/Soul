#include "slpch.h"
#include "Mesh.h"

namespace Soul
{
	Mesh::Mesh(std::vector<Vertex>& vert, std::vector<uint32_t>& ind, std::vector<Ref<Texture2D>>& texts)
	{
		vertices = vert;
		indices = ind;
		textures = texts;
		setupMesh();
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::BindTextures()
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			textures[i]->Bind(i);
		}
	}


	void Mesh::setupMesh()
	{
		shaderLib.Load("assets/shaders/Texture.glsl");


		vao = VertexArray::Create();
		vao->Bind();

		vbo = VertexBuffer::Create(vertices, vertices.size());
		vbo->SetLayout(
			{
				{ Soul::ShaderDataType::Float3, "a_Position" },
				{ Soul::ShaderDataType::Float3, "a_Normal" },
			   { Soul::ShaderDataType::Float2, "a_TexCoord" }
			}
		);
		vao->AddVertexBuffer(vbo);

		ebo = IndexBuffer::Create(indices.data(), indices.size());
		vao->SetIndexBuffer(ebo);
	}
}