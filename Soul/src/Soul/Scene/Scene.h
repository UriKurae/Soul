#pragma once

#include "Soul/Core/Timestep.h"
#include "Soul/Renderer/EditorCamera.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Soul/Renderer/Renderer.h" // TODO: Move this to cpp
#include "Soul/Renderer/Texture.h"
#include "entt.hpp"

class VertexBuffer;

namespace Soul
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
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


		entt::registry m_Registry;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}