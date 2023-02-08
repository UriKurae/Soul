#pragma once

#include "glm/glm.hpp"

namespace Soul
{
	enum class LightType
	{
		DIRECTIONAL_LIGHT = 0,
		POINT_LIGHT,
	};
	class Light
	{
	public:
		Light(glm::vec4 colorRGBA, float amount, LightType lightType) : color(colorRGBA), 
			intensity(amount), type(lightType), ambient(0.1f), diffuse(1.0f), specular(0.5f)
		{
		};
		virtual ~Light() {};

		virtual void SetColor(glm::vec4 colorRGBA) = 0;
		virtual void SetIntensity(float amount) = 0;

		virtual glm::vec4 GetColor() const { return color; }
		virtual glm::vec3 GetAmbient() const { return ambient; }
		virtual glm::vec3 GetDiffuse() const { return diffuse; }
		virtual glm::vec3 GetSpecular() const { return specular; }
		virtual float GetIntensity() const { return intensity; }
		virtual LightType GetType() const { return type; }

	protected:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec4 color;
		float intensity;
		LightType type;
	};

}