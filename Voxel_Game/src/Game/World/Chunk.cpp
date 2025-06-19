#include "Chunk.h"

namespace Game
{
	using namespace Arcane;

	void Chunk::Init()
	{
		for (auto y = 0; y < MAX_HEIGHT; ++y)
			for (auto x = 0; x < MAX_DEPTH; ++x)
				for (auto z = 0; z < MAX_WIDTH; ++z)
					m_Blocks[x][y][z] = BlockId::AIR;
	}

}