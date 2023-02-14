#include "slpch.h"
#include "CubeMap.h"
#include "Soul/Renderer/Renderer.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Soul
{
	CubeMap::CubeMap()
	{
        skyboxVertices = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        SetupCubeMap(skyboxVertices.data());
       
	}

	CubeMap::~CubeMap()
	{

	}

    void CubeMap::Draw()
    {
        glDepthFunc(GL_LEQUAL);
        glActiveTexture(cubeMapTexture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        Renderer::SubmitArrays(cubeMapShader, vao, 36);
        glDepthFunc(GL_LESS);
    }

    void CubeMap::SetupCubeMap(float* vertices)
    {
        vao = VertexArray::Create();
        
        vbo = VertexBuffer::Create(vertices, skyboxVertices.size() * sizeof(float));
        vbo->SetLayout(
            {
                { ShaderDataType::Float3, "a_Position" }
            }
        );

        vao->AddVertexBuffer(vbo);

        cubeMapShader = shaderLib.Load("assets/shaders/CubeMap.glsl");

        cubeMapFaces =
        {
            "assets/CubeMaps/skybox/right.jpg",
            "assets/CubeMaps/skybox/left.jpg",
            "assets/CubeMaps/skybox/top.jpg",
            "assets/CubeMaps/skybox/bottom.jpg",
            "assets/CubeMaps/skybox/front.jpg",
            "assets/CubeMaps/skybox/back.jpg"
        };
       cubeMapTexture = LoadCubeMap(cubeMapFaces);
    }

    unsigned int CubeMap::LoadCubeMap(std::vector<std::string> faces)
	{
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                SL_CORE_ERROR("CubeMap texture failed to load at path: {0}", faces[i]);
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
	}
}