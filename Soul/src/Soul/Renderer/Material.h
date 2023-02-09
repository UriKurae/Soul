#pragma once
#include <Soul/Renderer/Texture.h>
#include "glm/glm.hpp"

namespace Soul
{
	class Material
	{
	public:
		Material();
		~Material();
	private:

		glm::vec3 ambient;
		Ref<Texture2D> diffuse;
		Ref<Texture2D> specular;

	};

}