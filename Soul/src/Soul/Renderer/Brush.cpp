#include "slpch.h"
#include "Brush.h"

namespace Soul
{
	Brush::Brush()
	{
	}
	Brush::~Brush()
	{
	}

	uint32_t Brush::RGBToPixel(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		uint32_t targetColor;
		unsigned char* colors = (unsigned char*)&targetColor;
		colors[0] = r;
		colors[1] = g;
		colors[2] = b;
		colors[3] = a;

		return targetColor;
	}

	bool Brush::PaintTexture(Ref<Texture2D> texture, glm::vec2 uvCoords)
	{
		uint32_t color = RGBToPixel(r,g,b,a);
		int pixelX, pixelY;
		UvToPixel(uvCoords, texture->GetWidth(), texture->GetHeight(), pixelX, pixelY);
		texture->Lock();
		
		texture->SetPixel32(pixelX, pixelY, color);

		texture->Unlock();

		return true;
	}

	void Brush::UvToPixel(glm::vec2 uvCoords, int width, int height, int& pixelX, int& pixelY)
	{
		glm::vec2 pixels = {};
		pixels = uvCoords;
		pixels.x *= width;
		pixels.y *= height;
		pixelX = int(pixels.x);
		pixelY = int(pixels.y);
	}
}