#pragma once

#include <Arcane/Render/VertexArray.h>

#include "Block.h"

namespace Game
{
	class Chunk
	{
	public:
		Chunk();
		~Chunk();

		inline BlockId GetBlockId(const glm::vec3& pos) { m_Blocks[(int)pos.x][(int)pos.y][(int)pos.z]; }

		inline glm::vec3& GetPosition() { return m_Position; }
		inline const glm::vec3& GetPosition() const { return m_Position; }

	private:
		void Init_Blocks();

	public:
		static const uint32_t MAX_DEPTH  = 16,
							  MAX_WIDTH  = 16,
							  MAX_HEIGHT = 256;

	private:
		Arcane::Shared<Arcane::VertexArray> m_VAO;

		uint32_t m_Blocks[MAX_DEPTH][MAX_HEIGHT][MAX_WIDTH];
		glm::vec3 m_Position;
	};

	static Arcane::Shared<Arcane::VertexArray> Create_Block();
}