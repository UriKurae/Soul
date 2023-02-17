#include "slpch.h"
#include "Point.h"

namespace Soul
{
	
	PointLight::~PointLight()
	{

	}

	void PointLight::SetRange(float amount)
	{
		range = amount;
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