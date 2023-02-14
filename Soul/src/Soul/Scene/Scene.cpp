#include "slpch.h"
#include "Scene.h"

#include "Components.h"
#include "Lights/Point.h"
#include "Lights/DirectionalLight.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Soul
{
	Scene::Scene()
	{
		skyBox = std::make_shared<CubeMap>();

		shaderExample = shaderLib.Load("assets/shaders/Texture.glsl");
		// Setup shader uniforms
		shaderExample->Bind();
		shaderExample->UploadUniformInt("material.diffuse", 0);
		shaderExample->UploadUniformInt("material.specular", 1);
		shaderExample->UploadUniformInt("material.normal", 2);
	
		shaderExample->Unbind();



		//texture = Texture2D::Create("assets/textures/dog.jpg");
		//
		

		/*lightShader = shaderLib.Load("assets/shaders/LightObject.glsl");
		lightShader->Bind();
		lightShader->UploadUniformFloat3("objectColor", { 1.0f, 0.5f, 0.31f });
		lightShader->UploadUniformFloat3("lightColor", { 1.0f, 1.0f, 1.0f });
		Entity light = light = CreateEntity("Directional Light");
		light.AddComponent<LightComponent>();
		auto& meshTest = light.AddComponent<MeshComponent>();
		meshTest.model = std::make_shared<Model>("assets/Models/backpack/backpack.obj");*/
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

	Entity Scene::CreateLight(LightType lightType)
	{
		Entity entity = {};
		if (lightType == LightType::DIRECTIONAL_LIGHT)
		{
			entity = CreateEntity("Directional Light");
			auto& light = entity.AddComponent<LightComponent>();
			light.light = std::make_shared<DirectionalLight>();
		}
		else
		{
			entity = CreateEntity("Point Light");
			auto& light = entity.AddComponent<LightComponent>();
			light.light = std::make_shared<PointLight>();
		}

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

		auto& mat = entity.AddComponent<MaterialComponent>();
		mat.mat = std::make_shared<Material>();

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
			MaterialComponent& mat = m_Registry.get<MaterialComponent>(entity);
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

			shaderExample->Bind();

			shaderExample->UploadUniformFloat3("camPos", camera.GetPosition());
			shaderExample->UploadUniformFloat("material.shininess", 1.0f);

			UploadLightUniforms(shaderExample);
			
			mat.mat->BindTextures();
			mesh.model->Draw(shaderExample, transform.GetTransform());
		}

		skyBox->Draw();

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

	void Scene::UploadLightUniforms(Ref<Shader> desiredShader)
	{
		auto lights = m_Registry.view<LightComponent>();
		desiredShader->UploadUniformInt("totalPointLights", RetieveTotalLights(LightType::POINT_LIGHT));

		int index = 0;
		for (auto light : lights)
		{
			LightComponent& lightComp = lights.get<LightComponent>(light);
			TransformComponent& lightTransform = m_Registry.get<TransformComponent>(light);
			Ref<PointLight> point = std::dynamic_pointer_cast<PointLight>(lightComp.light);
			if (point)
			{
				std::string num = std::to_string(index);
				desiredShader->UploadUniformFloat3(("pointLights[" + num + "].position"), lightTransform.translation);
				desiredShader->UploadUniformFloat3(("pointLights[" + num + "].ambient"), point->GetAmbient());
				desiredShader->UploadUniformFloat3(("pointLights[" + num + "].diffuse"), point->GetDiffuse());
				desiredShader->UploadUniformFloat3(("pointLights[" + num + "].specular"), point->GetSpecular());
				desiredShader->UploadUniformFloat(("pointLights[" + num + "].constant"), point->GetConstant());
				desiredShader->UploadUniformFloat(("pointLights[" + num + "].linear"), point->GetLinear());
				desiredShader->UploadUniformFloat(("pointLights[" + num + "].quadratic"), point->GetQuadratic());
				index++;
			}

			Ref<DirectionalLight> DirLight = std::dynamic_pointer_cast<DirectionalLight>(lightComp.light);
			if (DirLight)
			{
				std::string str = "dirLight.";
				desiredShader->UploadUniformFloat3(str + "direction", glm::normalize(lightTransform.rotation));
				desiredShader->UploadUniformFloat3(str + "ambient", DirLight->GetAmbient());
				desiredShader->UploadUniformFloat3(str + "diffuse", DirLight->GetDiffuse());
				desiredShader->UploadUniformFloat3(str + "specular", DirLight->GetSpecular());
			}
		}
	}

	int Scene::RetieveTotalLights(LightType type)
	{
		int total = 0;

		auto lights = m_Registry.view<LightComponent>();
		for (auto light : lights)
		{
			LightComponent& lightComp = lights.get<LightComponent>(light);
			if (lightComp.light->GetType() == type)
			{
				total++;
			}
		}

		return total;
	}

	// Must have one for each component, if there's nothing to do, put it but leave it blank
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<MaterialComponent>(Entity entity, MaterialComponent& component)
	{

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

	template<>
	void Scene::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
	{

	}
}