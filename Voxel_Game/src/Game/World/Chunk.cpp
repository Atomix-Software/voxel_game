#include "Chunk.h"

namespace Game
{
	Chunk::Chunk()
	{
		Init_Blocks();
	}

	Chunk::~Chunk()
	{

	}

	void Chunk::Init_Blocks()
	{
		for (auto y = 0; y < MAX_HEIGHT; ++y)
			for (auto x = 0; x < MAX_DEPTH; ++x)
				for (auto z = 0; z < MAX_WIDTH; ++z)
					m_Blocks[x][y][z] = BlockType::AIR;
	}

	void Create_Face()
	{

	}

	void Create_Block()
	{

	}
}