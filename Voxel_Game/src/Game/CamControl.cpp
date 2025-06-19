#include "CamControl.h"

#include <Arcane.h>

namespace Game
{
	using namespace Arcane;

	CamControl::CamControl(float width, float height, float fov) :
		m_FirstMove(true), m_Paused(false)
	{
		m_Speed = 25.0f;
		m_Sensitivity = 0.1f;

		m_Camera = CreateShared<GameCamera>(fov, width, height); 
	}

	void CamControl::OnUpdate(Timestep ts, bool paused)
	{
		m_Paused = paused;
		if (m_Paused) return;

		float velocity = ts * m_Speed;

		glm::vec3 forward = glm::normalize(glm::vec3(m_Camera->GetFront().x, 0.0f, m_Camera->GetFront().z));
		glm::vec3 right = glm::normalize(m_Camera->GetRight());

		glm::vec3 position = m_Camera->GetPosition();
		if (Input::IsKeyPressed(Key::W))
			position += forward * velocity;
		else if (Input::IsKeyPressed(Key::S))
			position -= forward * velocity;

		if (Input::IsKeyPressed(Key::A))
			position -= right * velocity;
		else if (Input::IsKeyPressed(Key::D))
			position += right * velocity;

		if (Input::IsKeyPressed(Key::Space)) // Jump
			position.y += velocity;
		else if (Input::IsKeyPressed(Key::LeftControl)) // Crouch
			position.y -= velocity;

		m_Camera->SetPosition(position);
	}

	void CamControl::OnEvent(Event& event)
	{
		EventDispatcher dispatch(event);
		dispatch.Dispatch<WindowResizeEvent>(ARC_BIND_EVENT_FN(CamControl::OnWindowResize));
		dispatch.Dispatch<MouseMovedEvent>(ARC_BIND_EVENT_FN(CamControl::OnMouseMoved));
	}

	bool CamControl::OnWindowResize(WindowResizeEvent& event)
	{
		m_Camera->Resize((float) event.GetWidth(), (float) event.GetHeight());
		return false;
	}

	bool CamControl::OnMouseMoved(MouseMovedEvent& event)
	{
		static float lastX = event.GetX(), lastY = event.GetY();

		float x = event.GetX();
		float y = event.GetY();

		if (m_FirstMove) {
			lastX = x;
			lastY = y;

			m_FirstMove = false;
		}

		// Calculate mouse delta
		float xOffset = x - lastX;
		float yOffset = lastY - y; // Inverted y-axis since screen coordinates start at top-left

		lastX = x;
		lastY = y;

		// Sensitivity adjustment
		xOffset *= m_Sensitivity;
		yOffset *= m_Sensitivity;

		// Update yaw and pitch
		glm::vec3 rotation = m_Camera->GetRotation();
		rotation.x += yOffset; // Pitch (vertical rotation)
		rotation.y += xOffset; // Yaw (horizontal rotation)

		// Constrain pitch to avoid flipping
		if (rotation.x > 89.0f) rotation.x = 89.0f;
		else if (rotation.x < -89.0f) rotation.x = -89.0f;

		if (rotation.y > 360.0f) rotation.y = 0.0f;
		else if (rotation.y < 0.0f) rotation.y = 360.0f;

		if (m_Paused) return false;

		// Apply updated rotation to the camera
		m_Camera->SetRotation(rotation);

		return false;
	}
}