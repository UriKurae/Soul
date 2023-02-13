#pragma once
#include <Platform/OpenGL/OpenGLVertexArray.h>
#include <Platform/OpenGL/OpenGLShader.h>


namespace Soul
{
	class CubeMap
	{
	public:
		CubeMap();
		~CubeMap();

		void Draw();

	private:
		void SetupCubeMap(float* vertices);
		unsigned int LoadCubeMap(std::vector<std::string> faces);

	private:
		std::vector<std::string> cubeMapFaces;
		unsigned int cubeMapTexture = 0;
		std::vector<float> skyboxVertices;

		Ref<VertexArray> vao;
		Ref<VertexBuffer> vbo;

		ShaderLibrary shaderLib;
		Ref<Shader> cubeMapShader;
	};

}