#include "slpch.h"
#include "Scene.h"

#include "Components.h"
#include "Soul/Renderer/Renderer.h"

#include <glm/glm.hpp>

#include "Entity.h"

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

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		// TODO: Render things
		Renderer::BeginScene(camera);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
			
		}

		Renderer::EndScene();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.primary)
				{
					mainCamera = &camera.camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			// TODO: Render things
			Renderer::BeginScene(mainCamera->GetProjection(), cameraTransform);

			auto view = m_Registry.view<TransformComponent>();
			for (auto entity : view)
			{
				TransformComponent& transform = view.get<TransformComponent>(entity);
			}

			Renderer::EndScene();
		}
	}

	// Must have one for each component, if there's nothing to do, put it but leave it blank
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}
}