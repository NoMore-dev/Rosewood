#pragma once

#include <Rosewood.h>

class ViewportPanel
{
public:
	ViewportPanel(Rosewood::Ref<Rosewood::Scene> scene);
	~ViewportPanel() = default;

	void OnImGuiRender();

	Rosewood::Ref<Rosewood::Framebuffer> GetRenderFrameBuffer()
	{
		return m_MultiSampledFramebuffer;
	}

private:
	Rosewood::Ref<Rosewood::Scene> m_Scene;
	Rosewood::Ref<Rosewood::Framebuffer> m_MultiSampledFramebuffer;
	Rosewood::Ref<Rosewood::Framebuffer> m_Framebuffer;
};