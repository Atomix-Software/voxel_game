#pragma once

#include <Arcane/Core/Timestep.h>
#include <Arcane/Events/ApplicationEvent.h>
#include <Arcane/Events/MouseEvent.h>

#include "Render/GameCamera.h"

namespace Game
{
	class CamControl
	{
	public:
		CamControl(float width, float height, float fov = 67.0f);
		~CamControl() {}

		void OnUpdate(Arcane::Timestep ts);
		void OnEvent(Arcane::Event& event);

		inline void SetSpeed(float speed) { m_Speed = speed; }
		inline void SetSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }

		inline Arcane::Shared<GameCamera>& GetCamera() { return m_Camera; }

		inline float GetSpeed() { return m_Speed; }
		inline float GetSensitivity() { return m_Sensitivity; }

	private:
		bool OnWindowResize(Arcane::WindowResizeEvent& event);
		bool OnMouseMoved(Arcane::MouseMovedEvent& event);

	private:
		Arcane::Shared<GameCamera> m_Camera;
		bool m_FirstMove;

		float m_Speed, m_Sensitivity;
	};
}