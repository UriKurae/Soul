#pragma once
#include <Soul/Renderer/Texture.h>
#include "Soul/Core/Timestep.h"
#include "glm/glm.hpp"

namespace Soul
{
	class Brush
	{
	public:
		Brush();
		~Brush();

		uint32_t RGBToPixel(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
		bool PaintTexture(Ref<Texture2D> texture, glm::vec2 uvCoords);
		bool PaintTextureUVs(Ref<Texture2D> texture, glm::vec2 pos, glm::vec2 viewportSize);

		float& GetBrushSize() { return brushSize; }
		glm::vec4 GetBrushColor()
		{
			return {r,g,b,a};
		}

		int GetCurrentTextureID() 
		{ 
			if (textureToPaint)
				return textureToPaint->GetRendererID(); 

			return -1;
		}

		void SetBrushColor(int red, int green, int blue, int alpha);

		void OnUpdate(Timestep dt);
	private:
		void UpdateTexture(Ref<Texture2D> texture);
		void UvToPixel(glm::vec2 uvCoords, int width, int height, int& pixelX, int& pixelY);
	private:

		bool canUpdateTexture = false;
		bool editTextureRequested = false;
		float updateFrequency = 0.25f;
		float actualFrequency = 0.0f;

		//Ref<Texture2D> brushTexture;
		Ref<Texture2D> textureToPaint;

		int r = 255, g = 0, b = 0, a = 255;
		float brushSize = 50.0f;

	};
}