#pragma once
#include "Light.h"

namespace Soul
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight() : Light({ 1.0f, 1.0f, 1.0f, 1.0f }, 0.1f, LightType::DIRECTIONAL_LIGHT) {};
		DirectionalLight(glm::vec4 colorRGBA, float amount, glm::vec3 dir, LightType lightType);
		~DirectionalLight();

		virtual void SetColor(glm::vec4 colorRGBA) override;
		virtual void SetIntensity(float amount) override;

		virtual glm::vec4 GetColor() const override;
		virtual float GetIntensity() const override;

	private:

		glm::vec3 direction;
	};
}