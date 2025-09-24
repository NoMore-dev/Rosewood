#include <imgui.h>
#include <imgui_internal.h>

#include "SceneHierachyPanel.h"

SceneHierachyPanel::SceneHierachyPanel(std::shared_ptr<SceneHierachy> sceneHierarchy)
	: m_Scene(sceneHierarchy->GetScene()), m_SceneHierarchy(sceneHierarchy)
{}

void SceneHierachyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy", false);

	Rosewood::EntityID selectedEntityID = m_SceneHierarchy->GetSelectedEntity();

	bool consumedClick = false;

	ImGuiTableFlags flags = ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("SceneHierachyTable", 2, flags))
	{
		ImGui::TableSetupColumn("c0", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("c1", ImGuiTableColumnFlags_WidthFixed, 20.0f);

		uint32_t i = 0;
		for (auto entityID : m_SceneHierarchy->GetEntitiesInHierarchy())
		{
			const std::string& entityTag = (m_Scene->GetRegistry()->GetComponent<Rosewood::TagComponent>(entityID)).Tag;

			ImGui::TableNextRow();

			ImGui::PushID(std::to_string((uint32_t)entityID).c_str());

			ImGui::TableSetColumnIndex(0);
			bool isOpenBefore = ImGui::TreeNodeUpdateNextOpen(ImGui::GetID(std::to_string((uint32_t)entityID).c_str()), 0);
			ImGuiTreeNodeFlags treeNodeFlags = (false ? 0 : ImGuiTreeNodeFlags_Leaf) | ImGuiTreeNodeFlags_OpenOnArrow | (selectedEntityID == entityID ? ImGuiTreeNodeFlags_Selected : 0);
			bool isOpen = ImGui::TreeNodeEx(std::to_string((uint32_t)entityID).c_str(), treeNodeFlags, entityTag.c_str());
			bool isOpenAfter = ImGui::TreeNodeUpdateNextOpen(ImGui::GetID(std::to_string((uint32_t)entityID).c_str()), 0);

			if (ImGui::IsItemClicked())
			{
				consumedClick = true;
				if (isOpenBefore == isOpenAfter)
					m_SceneHierarchy->SetSelectedEntity(entityID);
			}


			ImGui::TableSetColumnIndex(1);
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 basePadding = style.FramePadding;
			style.FramePadding = ImVec2(2.5f, 0.f);
		
			if (ImGui::Button("X"))
			{
				m_SceneHierarchy->RemoveEntityFromHierachy(i);
				m_Scene->GetRegistry()->DestroyEntity(entityID);
				i--;

				if (selectedEntityID == entityID)
					m_SceneHierarchy->DeselectSelectedEntity();
			}
			
			style.FramePadding = basePadding;


			if (isOpen)
			{
		
				ImGui::TreePop();
			}

			ImGui::PopID();

			i++;
		}

		ImGui::EndTable();
	}

	ImVec2 rectMin = ImGui::GetWindowContentRegionMin(); rectMin.x += ImGui::GetWindowPos().x; rectMin.y += ImGui::GetWindowPos().y;
	ImVec2 rectMax = ImGui::GetWindowContentRegionMax(); rectMax.x += ImGui::GetWindowPos().x; rectMax.y += ImGui::GetWindowPos().y;
	bool mouseInTreeRect = ImGui::IsMouseHoveringRect(rectMin, rectMax);
	bool mouseClicked = ImGui::IsMouseClicked(0);
	if (mouseInTreeRect && mouseClicked && !consumedClick && ImGui::IsWindowHovered())
		m_SceneHierarchy->DeselectSelectedEntity();


	ImGui::End();
}
  