#include "slpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include "stb_image_write.h"

#include <glad/glad.h>

namespace Soul
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
	{
		pixels = nullptr;
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		SL_CORE_ASSERT(data, "Failed to load image");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_SRGB8_ALPHA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_SRGB8;
			dataFormat = GL_RGB;
		}

		SL_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	{
		pixels = nullptr;
		m_Width = width;
		m_Height = height;
		glGenTextures(1, &m_RendererID);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
			GL_FLOAT, NULL);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
		
		if (pixels)
		{
			delete[] pixels;
			pixels = nullptr;
		}
	}

	std::string OpenGLTexture2D::GetName() const
	{
		std::string name = m_Path.substr(m_Path.find_last_of("/") + 1, std::string::npos);

		return name;
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::BindToCompute(uint32_t slot) const
	{
		glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}

	bool OpenGLTexture2D::GenerateImagePNG(std::string pathToGenerate) const
	{
		pathToGenerate += ".png";
		GLuint size = m_Width * m_Height;
		unsigned int* data = new GLuint[size];

		//Set current texture
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		//Get pixels
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		int success = stbi_write_png(pathToGenerate.c_str(), m_Width, m_Height, 4, data, 4 * m_Width);

		//Unbind texture
		glBindTexture(GL_TEXTURE_2D, NULL);

		return success == 0 ? false : true;
	}

	bool OpenGLTexture2D::Unlock()
	{
		//If texture is locked and a texture exists
		//if (pixels != nullptr && m_RendererID != 0)
		//{
		//	//Set current texture
		//	glBindTexture(GL_TEXTURE_2D, m_RendererID);
		//
		//	//Update texture
		//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		//
		//	//Delete pixels
		//	delete[] pixels;
		//	pixels = nullptr;
		//
		//	//Unbind texture
		//	glBindTexture(GL_TEXTURE_2D, NULL);
		//
		//	return true;
		//}

		return false;
	}

	bool OpenGLTexture2D::Lock()
	{
		if (pixels == nullptr && m_RendererID != 0)
		{
			GLuint size = m_Width * m_Height;
			pixels = new GLuint[size];

			//Set current texture
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			//Get pixels
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			//Unbind texture
			glBindTexture(GL_TEXTURE_2D, NULL);

			return true;
		}

		return false;
	}

	uint32_t* OpenGLTexture2D::GetPixelData32()
	{
		return pixels;
	}

	uint32_t OpenGLTexture2D::GetPixel32(uint32_t x, uint32_t y)
	{
		return pixels[y * m_Width + x];
	}

	void OpenGLTexture2D::SetPixel32(uint32_t x, uint32_t y, uint32_t pixel)
	{
		if (x >= m_Width || x < 0) return;
		if (y >= m_Height || y < 0) return;
		pixels[y * m_Width + x] = pixel;
	}
}
