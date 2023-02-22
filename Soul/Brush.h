#pragma once
#include <Soul/Renderer/Texture.h>
#include "glm/glm.hpp"

namespace Soul
{
	class Brush
	{
	public:
		Brush();
		~Brush();

	private:

		//Ref<Texture2D> brushTexture;

		glm::vec4 brushColor = { 0.0f, 1.0f, 0.0f, 1.0f };

	};
}