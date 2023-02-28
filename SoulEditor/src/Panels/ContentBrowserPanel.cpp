#include "slpch.h"
#include "ContentBrowserPanel.h"

#include <ImGui/imgui.h>

namespace Soul
{
	extern const std::filesystem::path assetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel() : currDirectory(assetsPath)
	{
		directoryIcon = Texture2D::Create("Panels/DirectoryIcon.png");
		fileIcon = Texture2D::Create("Panels/FileIcon.png");
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{

	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (currDirectory != std::filesystem::path(assetsPath))
		{
			if (ImGui::Button("<-"))
			{
				currDirectory = currDirectory.parent_path();
			}
		}

		static float padding = 15.0f;
		static float thumbnailSize = 40.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(currDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, assetsPath);
			std::string filenameString = relativePath.filename().string();

			ImGui::PushID(filenameString.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? directoryIcon : fileIcon;
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("Content_Browser_Item", itemPath, (wcslen(itemPath)+1) * sizeof(wchar_t), ImGuiCond_Once);

				ImGui::EndDragDropSource();
			}
			
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					currDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		// TODO: status bar
		ImGui::End();
	}
}