#include "slpch.h"
#include "ComputeShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Soul
{
	
	ComputeShader::ComputeShader(const std::string& filepath)
	{
		shaderCode = ReadFile(filepath);
		
		// Extract name from filepath, for example assets/texture.glsl will extract texture
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;

		m_Name = filepath.substr(lastSlash, count);

		Compute();
	}


	ComputeShader::~ComputeShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void ComputeShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}


	void ComputeShader::Unbind() const
	{
		glUseProgram(0);
	}

	void ComputeShader::Compute()
	{
		const char* code = shaderCode.c_str();
		GLuint shader = 0;
		shader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			SL_CORE_WARN("Error {0}", infoLog);
		}

	
		// shader Program
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, shader);
		glLinkProgram(m_RendererID);
		glValidateProgram(m_RendererID);
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
			SL_CORE_ERROR("Compute Shader linking failed {0}", infoLog);
		}
		else
		{
			SL_CORE_INFO("Compute Shader linked successfully!");
		}

		glDetachShader(m_RendererID, shader);
		glDeleteShader(shader);

	}

	void ComputeShader::Dispatch()
	{
		glDispatchCompute(512, 512, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	void ComputeShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void ComputeShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void ComputeShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void ComputeShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void ComputeShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void ComputeShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ComputeShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		Bind();
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	std::string ComputeShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			SL_CORE_ERROR("Couldn't open file '{0}'", filepath);
		}

		return result;
	}
	
}