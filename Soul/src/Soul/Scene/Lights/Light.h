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
		Light(LightType lightType) : type(lightType), ambient(0.1f), diffuse(1.0f), specular(0.5f)
		{
		};
		virtual ~Light() {};

		virtual glm::vec3 GetColor() const { return ambient; }
		virtual glm::vec3 GetDiffuse() const { return diffuse; }
		virtual glm::vec3 GetSpecular() const { return specular; }

		virtual void SetColor(glm::vec4 color) 
		{ 
			ambient = { color.r, color.g, color.b };
			oldAmbient = ambient;
		}
		virtual void SetIntensity(float level) 
		{
			ambient = level * oldAmbient;
		}
		

		virtual LightType GetType() const { return type; }

	protected:
		glm::vec3 ambient;
		glm::vec3 oldAmbient = { 1.0f, 1.0f, 1.0f };
		glm::vec3 diffuse;
		glm::vec3 specular;
	
		LightType type;
	};

}