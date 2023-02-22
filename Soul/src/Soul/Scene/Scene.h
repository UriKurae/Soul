#pragma once

#include "Soul/Core/Timestep.h"
#include "Soul/Renderer/EditorCamera.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Soul/Renderer/Renderer.h" // TODO: Move this to cpp
#include "Soul/Renderer/Texture.h"
#include <Soul/Renderer/CubeMap.h>
#include "Soul/Scene/Components.h"
#include "entt.hpp"


namespace Soul
{
	class VertexBuffer;
	class Model;
	class Entity;
	enum class LightType;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateLight(LightType lightType);
		Entity ImportModel(const std::string& nam);
		void DestroyEntity(Entity entity);

		void UploadLightUniforms(Ref<Shader> desiredShader);
		int RetieveTotalLights(LightType type);

		void SetSceneExposure(float level) { sceneExposure = level; }
		float* GetSceneExposure() { return &sceneExposure; }

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
	public:
		std::shared_ptr<Model> currentModel;
		Ref<Shader> textureShader;
		TransformComponent modelTransform = {};
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:


		ShaderLibrary shaderLib;

		std::shared_ptr<CubeMap> skyBox;

		float sceneExposure = 25.0f;

		Ref<Shader> lightShader;

		entt::registry m_Registry;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}