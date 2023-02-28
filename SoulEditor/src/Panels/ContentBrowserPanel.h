#pragma once

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
	};
}