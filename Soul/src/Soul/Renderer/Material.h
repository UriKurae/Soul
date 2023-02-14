#pragma once
#include <Soul/Renderer/Texture.h>
#include "glm/glm.hpp"

namespace Soul
{
	class Material
	{
	public:
		Material();
		~Material();

		void BindTextures() const;
		void AddDiffuse(std::string path);
		void AddSpecular(std::string path);
		void AddNormal(std::string path);
	public:

		glm::vec3 ambient;
		Ref<Texture2D> diffuse;
		Ref<Texture2D> specular;
		Ref<Texture2D> normal;

	};

}