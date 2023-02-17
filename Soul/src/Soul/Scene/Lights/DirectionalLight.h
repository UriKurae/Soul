#pragma once
#include "Light.h"

namespace Soul
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight() : Light(LightType::DIRECTIONAL_LIGHT) {};
		
		~DirectionalLight();

	private:

		glm::vec3 direction;
	};
}