#pragma once

#include "entt.hpp"

namespace Soul
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	private:
		entt::registry m_Registry;
	};
}