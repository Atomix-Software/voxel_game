#pragma once

#include <Arcane/Render/Camera.h>

namespace Game
{
	namespace CameraConsts {
		static const glm::vec3 WORLD_UP = { 0.0f, 1.0f, 0.0f };
	}

	class GameCamera : public Arcane::Camera
	{
	public:
		GameCamera(float fov, float width, float height);

		inline void Resize(float width, float height)
		{
			m_Width = width;
			m_Height = height;
			m_Projection = glm::perspectiveFov(glm::radians(m_FOV), m_Width, m_Height, 0.01f, 1000.0f);
		}

		inline float GetFOV() { return m_FOV; }

		inline float GetWidth() { return m_Width; }
		inline float GetHeight() { return m_Height; }
		inline float GetAspectRatio() { return m_Width / m_Height; }

	private:
		virtual void UpdateMatrices() override;

	private:
		float m_FOV;
		float m_Width, m_Height;

		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_Front;
	};
}