#pragma once

#include "Soul/Renderer/Texture.h"
#include <filesystem>


namespace Soul
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path currDirectory;
		Ref<Texture2D> directoryIcon;
		Ref<Texture2D> fileIcon;
	};
}