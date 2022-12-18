#include "slpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

namespace Soul
{
	Scene::Scene()
	{
		struct MeshComponent
		{
			MeshComponent() = default;
		};
		struct TransformComponent
		{
			glm::mat4 transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& trans)
				: transform(trans) {}

			operator glm::mat4& () { return transform; }
			operator const glm::mat4& () const { return transform; }
		};


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
			//auto&[tran, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
	}

	Scene::~Scene()
	{

	}
}