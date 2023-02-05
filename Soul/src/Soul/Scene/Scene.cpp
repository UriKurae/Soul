#include "slpch.h"
#include "Scene.h"

#include "Components.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Soul
{
	Scene::Scene()
	{
		/*vao = VertexArray::Create();
		vao->Bind();

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		vbo = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		vbo->SetLayout({
			   { Soul::ShaderDataType::Float3, "a_Position" },
			   { Soul::ShaderDataType::Float2, "a_TexCoord" }			   
			});
		vao->AddVertexBuffer(vbo);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ebo = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		vao->SetIndexBuffer(ebo);*/

		shaderExample = shaderLib.Load("assets/shaders/Texture.glsl");
		//
		shaderExample->Bind();
		shaderExample->UploadUniformInt("u_Texture", 0);
		//texture = Texture2D::Create("assets/textures/dog.jpg");
		//
		
		//
		//int a = 0;
		//a++;
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

	Entity Scene::ImportModel(const std::string& path)
	{
		currentModel = std::make_shared<Model>(path);

		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		int index = path.find_last_of('/');
		tag.tag = path.substr(index+1, path.find_last_of('.') - (index + 1));

		auto& mesh = entity.AddComponent<MeshComponent>();
		mesh.model = currentModel;

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

		auto view = m_Registry.view<MeshComponent>();
		for (auto entity : view)
		{
			MeshComponent& mesh = view.get<MeshComponent>(entity);
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
			
			mesh.model->Draw(shaderExample, transform.GetTransform());
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
	void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
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