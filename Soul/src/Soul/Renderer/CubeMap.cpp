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
        cubeMapShader->Unbind();
    }

    void CubeMap::ChangeCubeMap(int index)
    {
        switch (index)
        {
        case 0:
        {
            cubeMapFaces =
            {
                 "assets/CubeMaps/field/right.jpg",
                "assets/CubeMaps/field/left.jpg",
                "assets/CubeMaps/field/top.jpg",
                "assets/CubeMaps/field/bottom.jpg",
                "assets/CubeMaps/field/front.jpg",
                "assets/CubeMaps/field/back.jpg"

                
            };
            break;
        }
        case 1:
        {
            cubeMapFaces =
            {
                "assets/CubeMaps/Yokohama/right.jpg",
                "assets/CubeMaps/Yokohama/left.jpg",
                "assets/CubeMaps/Yokohama/top.jpg",
                "assets/CubeMaps/Yokohama/bottom.jpg",
                "assets/CubeMaps/Yokohama/front.jpg",
                "assets/CubeMaps/Yokohama/back.jpg"
            };
            break;
        }
        case 2:
        {
            cubeMapFaces =
            {
               "assets/CubeMaps/skybox/right.jpg",
                "assets/CubeMaps/skybox/left.jpg",
                "assets/CubeMaps/skybox/top.jpg",
                "assets/CubeMaps/skybox/bottom.jpg",
                "assets/CubeMaps/skybox/front.jpg",
                "assets/CubeMaps/skybox/back.jpg"
            };
            break;
        }
        case 3:
        {
            cubeMapFaces =
            {
                "assets/CubeMaps/HotelSea/right.jpg",
                "assets/CubeMaps/HotelSea/left.jpg",
                "assets/CubeMaps/HotelSea/top.jpg",
                "assets/CubeMaps/HotelSea/bottom.jpg",
                "assets/CubeMaps/HotelSea/front.jpg",
                "assets/CubeMaps/HotelSea/back.jpg"
            };
            break;
        }
        case 4:
        {
            cubeMapFaces =
            {
                "assets/CubeMaps/Basilica/right.jpg",
                "assets/CubeMaps/Basilica/left.jpg",
                "assets/CubeMaps/Basilica/top.jpg",
                "assets/CubeMaps/Basilica/bottom.jpg",
                "assets/CubeMaps/Basilica/front.jpg",
                "assets/CubeMaps/Basilica/back.jpg"
            };
            break;
        }
        case 5:
        {
            cubeMapFaces =
            {
                "assets/CubeMaps/Italy/right.jpg",
                "assets/CubeMaps/Italy/left.jpg",
                "assets/CubeMaps/Italy/top.jpg",
                "assets/CubeMaps/Italy/bottom.jpg",
                "assets/CubeMaps/Italy/front.jpg",
                "assets/CubeMaps/Italy/back.jpg"
            };
            break;
        }
            
        }
        cubeMapTexture = LoadCubeMap(cubeMapFaces);
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
            "assets/CubeMaps/field/right.jpg",
            "assets/CubeMaps/field/left.jpg",
            "assets/CubeMaps/field/top.jpg",
            "assets/CubeMaps/field/bottom.jpg",
            "assets/CubeMaps/field/front.jpg",
            "assets/CubeMaps/field/back.jpg"
        };
       cubeMapTexture = LoadCubeMap(cubeMapFaces);
    }



    unsigned int CubeMap::LoadCubeMap(std::vector<std::string> faces)
	{
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        stbi_set_flip_vertically_on_load(0);
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