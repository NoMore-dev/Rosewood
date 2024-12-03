#pragma once

#include "Rosewood/Input/Input.h"

namespace Rosewood {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

		virtual void InitImpl() override;
		virtual void UpdateMousePositionDelta() override;

	private:
		std::pair<float, float> m_LastFrameMousePosition;
	};

}
