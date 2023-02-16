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
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float3, "a_Tangent"},
				{ ShaderDataType::Float3, "a_Bitangent"}
			}
		);
		vao->AddVertexBuffer(vbo);

		ebo = IndexBuffer::Create(indices.data(), indices.size());
		vao->SetIndexBuffer(ebo);
	}
}