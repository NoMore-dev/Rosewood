
#include <imgui.h>
#include <imgui_internal.h>

#include "Inspector.h"

Inspector::Inspector(std::shared_ptr<SceneHierachy> sceneHierarchy)
	: m_Scene(sceneHierarchy->GetScene()), m_SceneHierarchy(sceneHierarchy) {}

void Inspector::OnImGuiRender()
{
	Rosewood::EntityID selectedEntityID = m_SceneHierarchy->GetSelectedEntity();

	ImGui::Begin("Inspector");

	if (selectedEntityID != Rosewood::Entity::null)
	{
		ImGui::Separator();

		if (m_Scene->GetRegistry()->HasComponent<Rosewood::TransformComponent>(selectedEntityID))
		{
			Rosewood::TransformData& transformData = m_Scene->GetRegistry()->GetComponent<Rosewood::TransformComponent>(selectedEntityID).Transform;
			DrawTransformComponentInspector(transformData);
		}

		if (m_Scene->GetRegistry()->HasComponent<Rosewood::RenderableObject3D>(selectedEntityID))
		{
			Rosewood::RenderableObject3D& renderableObject = m_Scene->GetRegistry()->GetComponent<Rosewood::RenderableObject3D>(selectedEntityID);
			DrawRenderableObject3DComponentInspector(renderableObject);
		}
	}

	ImGui::End();
}

void Inspector::DrawTransformComponentInspector(Rosewood::TransformData& transformData)
{
	const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx("Transform", treeFlags))
	{
		glm::vec3 position = transformData.GetTranslation();
		glm::vec3 rotation = transformData.GetRotationEuler();
		float scale = transformData.GetScale().x;

		ImGuiTableFlags flags = ImGuiTableFlags_NoBordersInBody;
		if (ImGui::BeginTable("TransformParametersTable", 2, flags))
		{
			ImGui::TableSetupColumn("c0", ImGuiTableColumnFlags_WidthFixed, 100.f);
			ImGui::TableSetupColumn("c1", ImGuiTableColumnFlags_WidthStretch);

			ImU32 bgColor = ImGui::GetColorU32(ImVec4(0.6f, 0.6f, 0.6f, .2f));

			ImGui::TableNextRow();
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, bgColor);
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("	Position");
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(":	");
			ImGui::SameLine();
			ImGui::DragFloat3("##Position", (float*)&position, 0.1f, -9999.f, 9999.f, "%.1f");

			ImGui::TableNextRow();
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, bgColor);
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("	Rotation");
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(":	");
			ImGui::SameLine();
			ImGui::DragFloat3("##Rotation", (float*)&rotation, 0.02f, -9999.f, 9999.f, "%.2f");

			ImGui::TableNextRow();
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, bgColor);
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("	Scale");
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(":	");
			ImGui::SameLine();
			ImGui::DragFloat("##Scale", &scale, 0.01f, 0.f, 9999.f, "%.2f");

			ImGui::EndTable();
		}

		transformData.SetTranslation(position);
		transformData.SetRotationEuler(rotation);
		transformData.SetScale(glm::vec3(scale));

		ImGui::TreePop();
	}

	ImGui::Separator();
}

void Inspector::DrawRenderableObject3DComponentInspector(Rosewood::RenderableObject3D& renderableObject)
{
	const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx("RenderableObject3D", treeFlags))
	{
		if (ImGui::TreeNodeEx("Materials", treeFlags))
		{
			for (int i = 0; i < renderableObject.Surfaces.size(); i++)
			{
				const ImGuiTreeNodeFlags treeFlags2 = ImGuiTreeNodeFlags_Bullet;
				if (ImGui::TreeNodeEx((renderableObject.SlotsNames[i] + "##materialname").c_str(), treeFlags2))
				{
					ImGuiTableFlags flags = ImGuiTableFlags_NoBordersInBody;
					if (ImGui::BeginTable("MaterialParametersTable", 2, flags))
					{
						ImGui::TableSetupColumn("c0", ImGuiTableColumnFlags_WidthFixed, 100.f);
						ImGui::TableSetupColumn("c1", ImGuiTableColumnFlags_WidthStretch);

						ImU32 bgColor = ImGui::GetColorU32(ImVec4(0.6f, 0.6f, 0.6f, .2f));

						auto material = renderableObject.Materials[i];
						std::vector<BYTE>& materialData = material->GetMaterialData();
						const Rosewood::BufferLayout& materialDataLayout = material->GetShader()->GetMaterialDataLayout();

						for (auto& element : materialDataLayout)
						{
							ImGui::TableNextRow();
							ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, bgColor);

							ImGui::TableSetColumnIndex(0);
							ImGui::Text(("	" + element.Name).c_str());

							ImGui::TableSetColumnIndex(1);
							ImGui::Text(":	");
							ImGui::SameLine();

							const ImGuiColorEditFlags colorPickerFlags = ImGuiColorEditFlags_NoInputs;
							switch (element.Type)
							{
							case Rosewood::ShaderDataType::Float3:
								ImGui::ColorEdit3(("##" + element.Name + std::to_string(i)).c_str(), (float*)(materialData.data() + element.Offset), colorPickerFlags); break;
							case Rosewood::ShaderDataType::Float:
								ImGui::DragFloat(("##" + element.Name + std::to_string(i)).c_str(), (float*)(materialData.data() + element.Offset), 0.1f, 0.0f, 128.f, "%.1f"); break;
							default:
								break;
							}


						}

						ImGui::EndTable();
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	ImGui::Separator();
}
