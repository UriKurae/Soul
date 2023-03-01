#include "slpch.h"
#include "Material.h"

namespace Soul
{
	Material::Material()
	{
		ambient = { 0.1f, 0.1f, 0.1f };

		diffuse = Texture2D::Create("assets/Models/backpack/diffuse.jpg");
		specular = Texture2D::Create("assets/textures/defaultDiffuse.jpg");
		normal = Texture2D::Create("assets/textures/defaultNormal.jpg");
	}

	Material::~Material()
	{

	}

	void Material::BindTextures() const
	{
		//diffuse->Bind(0);
		specular->Bind(1);
		normal->Bind(2);
	}

	void Material::AddDiffuse(std::string path)
	{
		diffuse = Texture2D::Create(path);	
	}

	void Material::AddSpecular(std::string path)
	{
		specular = Texture2D::Create(path);
	}

	void Material::AddNormal(std::string path)
	{
		normal = Texture2D::Create(path);
	}
}