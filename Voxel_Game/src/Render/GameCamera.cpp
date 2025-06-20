#include "GameCamera.h"

namespace Game
{
	using namespace Arcane;

	GameCamera::GameCamera(float fov, float width, float height) :
		Camera(), m_FOV(fov), m_Width(width), m_Height(height)
	{
		Resize(m_Width, m_Height);
		m_Up = glm::vec3(CameraConsts::WORLD_UP);
	}

	void GameCamera::UpdateMatrices()
	{
		float yaw = glm::radians(m_Rotation.y); // Mouse X
		float pitch = glm::radians(m_Rotation.x); // Mouse Y

		glm::vec3 front;
		front.x = glm::cos(pitch) * glm::sin(yaw);
		front.y = glm::sin(pitch);
		front.z = glm::cos(pitch) * glm::cos(yaw);
		m_Front = glm::normalize(front);

		m_Right = glm::normalize(glm::cross(CameraConsts::WORLD_UP, m_Front));
		m_Up = glm::normalize(glm::cross(m_Front, m_Right));

		m_View = glm::lookAtLH(m_Position, m_Position + m_Front, CameraConsts::WORLD_UP);
	}

}