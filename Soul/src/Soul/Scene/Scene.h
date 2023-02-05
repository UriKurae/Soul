#pragma once

#include "Soul/Core/Timestep.h"
#include "Soul/Renderer/EditorCamera.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Soul/Renderer/Renderer.h" // TODO: Move this to cpp
#include "Soul/Renderer/Texture.h"
#include "entt.hpp"


namespace Soul
{
	class VertexBuffer;
	class Model;
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity ImportModel(const std::string& nam);
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:

		ShaderLibrary shaderLib;

		Ref<Shader> shaderExample;
		Ref<VertexArray> vao;
		Ref<VertexBuffer> vbo;
		Ref<IndexBuffer> ebo;
		Ref<Texture2D> texture;

		std::shared_ptr<Model> currentModel;


		entt::registry m_Registry;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}