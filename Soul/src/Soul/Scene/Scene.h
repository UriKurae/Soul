#pragma once

#include "entt.hpp"

#include "Soul/Core/Timestep.h"

namespace Soul
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// Temporary
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};
}