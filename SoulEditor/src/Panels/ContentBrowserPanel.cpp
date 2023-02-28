#include "slpch.h"
#include "ContentBrowserPanel.h"

#include <ImGui/imgui.h>

namespace Soul
{
	static const std::filesystem::path assetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel() : currDirectory(assetsPath)
	{
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{

	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (currDirectory != std::filesystem::path(assetsPath))
		{
			if (ImGui::Button("[Back]"))
			{
				currDirectory = currDirectory.parent_path();
			}
			ImGui::Separator();
		}

		for (auto& it : std::filesystem::directory_iterator(currDirectory))
		{
			const auto& path = it.path();
			auto relativePath = std::filesystem::relative(path, assetsPath);
			std::string filenameString = relativePath.filename().string();

			if (it.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					currDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{

				}
			}
		}

		ImGui::End();
	}
}