#include <imgui.h>

#include "ViewportPanel.h"

ViewportPanel::ViewportPanel(Rosewood::Ref<Rosewood::Scene> scene)
	: m_Scene(scene)
{
	Rosewood::FramebufferSpecification fbSpec;
		fbSpec.Height = 1280;
		fbSpec.Width = 720;
		fbSpec.Samples = 1;
		fbSpec.AttachmentsFormats = Rosewood::FramebufferAttachmentsFormats{ {Rosewood::AttachmentFormat::RGBA}, Rosewood::AttachmentFormat::NO_ATTACHEMENT };
	m_Framebuffer = Rosewood::Framebuffer::Create(fbSpec);

	Rosewood::FramebufferSpecification msfbSpec;
		msfbSpec.Height = 1280;
		msfbSpec.Width = 720;
		msfbSpec.Samples = 8;
		msfbSpec.AttachmentsFormats = Rosewood::FramebufferAttachmentsFormats{ {Rosewood::AttachmentFormat::RGBA} };
	m_MultiSampledFramebuffer = Rosewood::Framebuffer::Create(msfbSpec);


	Rosewood::Renderer::SetRenderFramebuffer(m_MultiSampledFramebuffer);
}

void ViewportPanel::OnImGuiRender()
{
	ImGui::Begin("Viewport");

	const Rosewood::PerspectiveCamera* const camera = (Rosewood::PerspectiveCamera*)m_Scene->GetRegistry()->GetComponent<Rosewood::CameraComponent>(m_Scene->GetPrimaryCameraID()).CameraRef.get();
	float aspectRatio = camera->GetSettings().ratio;

	ImVec2 viewportSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x * 1.f / aspectRatio);
	if (viewportSize.y > ImGui::GetContentRegionAvail().y)
		viewportSize = ImVec2(ImGui::GetContentRegionAvail().y * aspectRatio, ImGui::GetContentRegionAvail().y);

	ImGuiStyle& style = ImGui::GetStyle();
	float size = viewportSize.x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;
	float off = (avail - size) * 0.5f;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	m_Framebuffer->BlitFrom(m_MultiSampledFramebuffer);
	uint32_t textureID = m_Framebuffer->GetColorAttachmentID(0);
	ImGui::Image((void*)std::intptr_t(textureID), viewportSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	m_Framebuffer->Resize(uint32_t(viewportSize.x), uint32_t(viewportSize.y));
	m_MultiSampledFramebuffer->Resize(uint32_t(viewportSize.x), uint32_t(viewportSize.y));

	ImGui::End();
}
