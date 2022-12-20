#include "slpch.h"
#include "Scene.h"

#include "Components.h"

#include <glm/glm.hpp>

namespace Soul
{
	Scene::Scene()
	{
#if EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& trans = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [tran, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif // EXAMPLE_CODE

	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}
	}
}