#pragma once
#include "Light.h"

namespace Soul
{
	class PointLight : public Light
	{
	public:
		PointLight() : Light(LightType::POINT_LIGHT),
			range(10.0f), constant(1.0f), linear(0.09f), quadratic(0.032f)
		{
		};
		
		~PointLight();

		virtual void SetRange(float amount);

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