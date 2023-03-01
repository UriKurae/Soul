#pragma once

#include "Soul/Renderer/Shader.h"

namespace Soul
{
	class ComputeShader : public Shader
	{
	public:
		ComputeShader(const std::string& filepath);
		virtual ~ComputeShader();

		void Bind() const override;
		void Unbind() const override;

		void Compute();
		void Dispatch(uint32_t width, uint32_t height);

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
		std::string shaderCode;
		
	};
}