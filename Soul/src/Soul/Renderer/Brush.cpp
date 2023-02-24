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
		int size = texture->GetWidth() * texture->GetHeight();
		textureToPaint = texture;
		textureToPaint->Lock();
		
		textureToPaint->SetPixel32(pixelX, pixelY, color);
		
		

		editTextureRequested = true;

		return true;
	}

	void Brush::OnUpdate(Timestep dt)
	{
		actualFrequency += dt.GetSeconds();
		if (actualFrequency > updateFrequency)
		{
			if (editTextureRequested)
			{
				//textureToPaint->Unlock();
				editTextureRequested = false;
			}
			actualFrequency = 0.0f;
			
		}
	}

	void Brush::UpdateTexture(Ref<Texture2D> texture)
	{
		texture->Lock();
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