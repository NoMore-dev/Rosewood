#pragma once

#include "Rosewood/Core.h"

namespace Rosewood{

	class RW_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

		inline static std::pair<float, float> GetMousePositionDelta() { return s_Instance->s_MousePositionDelta; }

		inline static void Init() { s_Instance->InitImpl(); }
		inline static void OnUpdate() { s_Instance->UpdateMousePositionDelta(); }

	private:
		static Input* s_Instance;


	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		virtual void InitImpl() = 0;
		virtual void UpdateMousePositionDelta() = 0;

	protected:
		std::pair<float, float> s_MousePositionDelta;

	};

}