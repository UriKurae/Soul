#pragma once

#include "glm/glm.hpp"
#include "Soul/Renderer/Shader.h"

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
		Light(LightType lightType) : type(lightType), newAmbient(0.1f), ambient(0.1f), diffuse(1.0f), specular(0.5f), intensity(1.0f)
		{
		};
		virtual ~Light() {};

		virtual glm::vec3 GetColor() const { return newAmbient; }
		virtual glm::vec3 GetDiffuse() const { return diffuse; }
		virtual glm::vec3 GetSpecular() const { return specular; }

		virtual void SetColor(glm::vec4 color) 
		{ 
			ambient = glm::vec3(color.r, color.g, color.b);
			oldAmbient = ambient;

			newAmbient = ambient * intensity;

			lightShader->Bind();
			lightShader->UploadUniformFloat3("lightColor", newAmbient);
		}
		virtual void SetIntensity(float level) 
		{
			intensity = level;
			newAmbient = oldAmbient * intensity;

			lightShader->Bind();
			lightShader->UploadUniformFloat3("lightColor", newAmbient);
			
		}
		

		virtual LightType GetType() const { return type; }

	public:
		Ref<Shader> lightShader;
		float intensity = 1.0f;
	protected:
		glm::vec3 ambient;
		glm::vec3 oldAmbient = { 1.0f, 1.0f, 1.0f };
		glm::vec3 diffuse;
		glm::vec3 specular;

		glm::vec3 newAmbient;

	
		LightType type;
	};

}