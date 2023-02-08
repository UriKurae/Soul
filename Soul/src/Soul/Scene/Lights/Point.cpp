#include "slpch.h"
#include "Point.h"

namespace Soul
{
	PointLight::PointLight(glm::vec4 colorRGBA, float amount, float ran, LightType lightType) : Light(colorRGBA, amount, lightType), range(ran)
	{

	}

	PointLight::~PointLight()
	{

	}

	void PointLight::SetColor(glm::vec4 colorRGBA)
	{
		color = colorRGBA;
	}

	void PointLight::SetIntensity(float amount)
	{
		intensity = amount;
	}

	void PointLight::SetRange(float amount)
	{
		range = amount;
	}

	glm::vec4 PointLight::GetColor() const
	{
		return color;
	}

	float PointLight::GetIntensity() const
	{
		return intensity;
	}

	float PointLight::GetRange() const
	{
		return range;
	}
	float PointLight::GetConstant() const
	{
		return constant;
	}
	float PointLight::GetLinear() const
	{
		return linear;
	}
	float PointLight::GetQuadratic() const
	{
		return quadratic;
	}
}