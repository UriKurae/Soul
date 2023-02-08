#include "slpch.h"
#include "DirectionalLight.h"

namespace Soul
{
	DirectionalLight::DirectionalLight(glm::vec4 colorRGBA, float amount, glm::vec3 dir, LightType lightType) : Light(colorRGBA, amount, lightType), direction(dir)
	{

	}

	DirectionalLight::~DirectionalLight()
	{

	}

	void DirectionalLight::SetColor(glm::vec4 colorRGBA)
	{
		color = colorRGBA;
	}

	void DirectionalLight::SetIntensity(float amount)
	{
		intensity = amount;
	}

	glm::vec4 DirectionalLight::GetColor() const
	{
		return color;
	}

	float DirectionalLight::GetIntensity() const
	{
		return intensity;
	}
}