#pragma once

#include "Rosewood/Layer/Layer.h"
#include "Rosewood/Event/ApplicationEvent.h"
#include "Rosewood/Event/KeyEvent.h"
#include "Rosewood/Event/MouseEvent.h"

namespace Rosewood {

	class RW_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnBegin();
		void OnEnd();
	};

}