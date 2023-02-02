#include "slpch.h"
#include "Mesh.h"

namespace Soul
{
	Mesh::Mesh(std::vector<Vertex>& vert, std::vector<uint32_t>& ind)
	{
		vertices = vert;
		indices = ind;
		setupMesh();
	}

	Mesh::~Mesh()
	{

	}


	void Mesh::setupMesh()
	{
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