#include "slpch.h"
#include "Material.h"

namespace Soul
{
	Material::Material()
	{
		ambient = { 0.1f, 0.1f, 0.1f };

		diffuse = Texture2D::Create("assets/textures/defaultDiffuse.png");
		specular = Texture2D::Create("assets/textures/defaultDiffuse.png");
	}

	Material::~Material()
	{

	}


}