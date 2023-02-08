#pragma once
#include "Light.h"

namespace Soul
{
	class PointLight : public Light
	{
	public:
		PointLight() : Light({0.0f, 0.0f, 0.0f, 1.0f}, 0.1f, LightType::POINT_LIGHT),
			range(10.0f), constant(1.0f), linear(0.09f), quadratic(0.032f)
		{
		};
		PointLight(glm::vec4 colorRGBA, float amount, float ran, LightType lightType);
		~PointLight();

		virtual void SetColor(glm::vec4 colorRGBA) override;
		virtual void SetIntensity(float amount) override;
		virtual void SetRange(float amount);

		virtual glm::vec4 GetColor() const override;
		virtual float GetIntensity() const override;
		virtual float GetRange() const;
		virtual float GetConstant() const;
		virtual float GetLinear() const;
		virtual float GetQuadratic() const;

	private:

		float range;
		float constant;
		float linear;
		float quadratic;
	};
}