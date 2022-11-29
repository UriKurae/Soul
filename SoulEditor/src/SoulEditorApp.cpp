#include <Soul.h>
#include <Soul/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Soul
{
	class SoulEditor : public Application
	{
	public:
		SoulEditor() : Application("Soul Editor")
		{
			PushLayer(new EditorLayer());
		}

		~SoulEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new SoulEditor();
	}
}