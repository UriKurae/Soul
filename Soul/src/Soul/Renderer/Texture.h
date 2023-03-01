#pragma once

#include <string>

#include "Soul/Core/Core.h"

namespace Soul
{
	enum class TextureType
	{
		TEXT_DIFFUSE = 0,
		TEXT_SPECULAR,
		TEXT_NORMAL,
		TEXT_HEIGHT
	};
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual std::string GetPath() const = 0;
		virtual std::string GetName() const = 0;
		virtual void BindToCompute() const = 0;
		virtual bool Unlock() = 0;
		virtual bool Lock() = 0;
		virtual uint32_t* GetPixelData32() = 0;
		virtual uint32_t GetPixel32(uint32_t x, uint32_t y) = 0;
		virtual void SetPixel32(uint32_t x, uint32_t y, uint32_t pixel) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
	};
}