#pragma once

#include "Soul/Scene/Scene.h"
#include "Soul/Core/Log.h"
#include "Soul/Core/Core.h"
#include "Soul/Scene/Entity.h"

namespace Soul
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}