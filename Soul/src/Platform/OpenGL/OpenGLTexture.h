#pragma once

#include "Soul/Renderer/Texture.h"

namespace Soul
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();
		
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual std::string GetPath() const override { return m_Path; }
		virtual std::string GetName() const override;

		bool Unlock() override;
		bool Lock() override;
		uint32_t* GetPixelData32() override;
		uint32_t GetPixel32(uint32_t x, uint32_t y) override;
		void SetPixel32(uint32_t x, uint32_t y, uint32_t pixel) override;

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		std::string type;
		uint32_t m_RendererID;

		unsigned int* pixels;
	};
}