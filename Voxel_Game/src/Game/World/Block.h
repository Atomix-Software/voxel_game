#pragma once

#include <Arcane/Core/Base.h>
#include <glm/vec3.hpp>

namespace Game
{
	enum BlockType : uint32_t
	{
		AIR = 0,
	};

	class Block
	{
	public:
		Block();
		~Block();

		inline glm::vec3& GetPosition() { return m_Position; }
		inline const glm::vec3& GetPosition() const { return m_Position; }

		inline glm::vec3& GetRotation() { return m_Rotation; }
		inline const glm::vec3& GetRotation() const { return m_Rotation; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

	};
}