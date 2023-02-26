#include "SceneHierarchyPanel.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#include "glm/gtc/type_ptr.hpp"

#include "Soul/Scene/Components.h"

#include "Soul/Utils/PlatformUtils.h"

namespace Soul
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		
		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});
		
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::TreeNode("Create Light"))
			{
				if (ImGui::Selectable("Directional Light"))
				{
					m_Context->CreateLight(LightType::DIRECTIONAL_LIGHT);
				}

				if (ImGui::Selectable("Point Light"))
				{
					m_Context->CreateLight(LightType::POINT_LIGHT);
				}
				
				ImGui::TreePop();
			}
			
			ImGui::EndPopup();
		}


		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		
		ImGui::End();

		ImGui::Begin("Brush Properties");
		
		ImGui::Text("Brush Size");
		ImGui::SliderFloat("##Brush Size", &m_Context->currentBrush.GetBrushSize(), 1.0f, 500.0f, "%.2f");
		
		ImGui::Separator();

		static glm::vec4 brushColor = {};
		ImGui::Text("Brush Color");
		if (ImGui::ColorPicker4("##Brush Color", &brushColor[0]))
		{
			m_Context->currentBrush.SetBrushColor(brushColor.r * 255.0f, brushColor.g * 255.0f, brushColor.b * 255.0f, brushColor.a * 255.0f);
		}

		

		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)| ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9231943, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}
	
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		// Get all the fonts and then getting the first one which is bold in this case
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f, 4.0f });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight , lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Mesh Component") && !m_SelectionContext.HasComponent<MeshComponent>())
			{
				m_SelectionContext.AddComponent<MeshComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Material Component") && !m_SelectionContext.HasComponent<MaterialComponent>())
			{
				m_SelectionContext.AddComponent<MaterialComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();
	
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Position", component.translation);
				glm::vec3 rotation = glm::degrees(component.rotation);
				DrawVec3Control("Rotation", rotation);
				component.rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.scale, 1.0f);
			});

		if (entity.HasComponent<MeshComponent>())
		{
			DrawComponent<MeshComponent>("Mesh Component", entity, [](auto& component)
				{

				});
		}

		if (entity.HasComponent<MaterialComponent>())
		{
			
			DrawComponent<MaterialComponent>("Material Component", entity, [](auto& component)
				{
					ImGui::Text(("Albedo: " + component.mat->diffuse->GetName()).c_str());
					ImGui::SameLine();
					ImGui::PushID(0);
					if (ImGui::Button("Add"))
					{
						std::string filePath = FileDialogs::OpenFile("Supported Files(*.png, *.jpg)\0*.png;*.jpg;\0");
						if (!filePath.empty())
						{
							std::replace(filePath.begin(), filePath.end(), '\\', '/');
							component.mat->AddDiffuse(filePath);
						}
					}
					ImGui::PopID();

					ImGui::Text(("Specular: " + component.mat->specular->GetName()).c_str());
					ImGui::SameLine();
					ImGui::PushID(1);
					if (ImGui::Button("Add"))
					{
						std::string filePath = FileDialogs::OpenFile("Supported Files(*.png, *.jpg)\0*.png;*.jpg;\0");
						if (!filePath.empty())
						{
							std::replace(filePath.begin(), filePath.end(), '\\', '/');
							component.mat->AddSpecular(filePath);
						}
					}
					ImGui::PopID();

					ImGui::Text(("Normal: " + component.mat->normal->GetName()).c_str());
					ImGui::SameLine();
					ImGui::PushID(2);
					if (ImGui::Button("Add"))
					{
						std::string filePath = FileDialogs::OpenFile("Supported Files(*.png, *.jpg)\0*.png;*.jpg;\0");
						if (!filePath.empty())
						{
							std::replace(filePath.begin(), filePath.end(), '\\', '/');
							component.mat->AddNormal(filePath);
						}
					}
					ImGui::PopID();
				});
		}


		if (entity.HasComponent<LightComponent>())
		{
			DrawComponent<LightComponent>("Light", entity, [](auto& component)
				{
					static float intensity = 0.0f;
					if (ImGui::SliderFloat("Intensity",&intensity, 0.0f, 10.0f, "%.2f"))
					{
						component.light->SetIntensity(intensity);
					}

					static glm::vec4 color = {};
					if (ImGui::ColorPicker4("Color", &color[0]))
					{
						component.light->SetColor(color);
					}
					
				});
		}	
	}
}