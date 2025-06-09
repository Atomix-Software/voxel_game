#include "Block.h"

namespace Game
{
	using namespace Arcane;

#pragma region Blocks Database Class

	std::unordered_map<BlockId, BlockFaceTextures> Blocks::s_BlockTextures;
	Shared<Texture2D> Blocks::s_AtlasTexture;

	void Blocks::Init() {
		s_AtlasTexture = Texture2D::Create("assets/textures/blocks.png");

		const glm::vec2 cellSize = { 16.0f, 16.0f };
		const glm::vec2 spriteSize = { 1.0f, 1.0f };

		BlockFaceTextures grassTextures;
		grassTextures.Top    = SubTexture2D::CreateFromCoords(s_AtlasTexture, { 0, 0 }, spriteSize, cellSize);
		grassTextures.Bottom = SubTexture2D::CreateFromCoords(s_AtlasTexture, { 2, 0 }, spriteSize, cellSize);
		grassTextures.Front  = SubTexture2D::CreateFromCoords(s_AtlasTexture, { 1, 0 }, spriteSize, cellSize);
		grassTextures.Back   = grassTextures.Front;
		grassTextures.Left   = grassTextures.Front;
		grassTextures.Right  = grassTextures.Front;

		s_BlockTextures[BlockId::GRASS] = grassTextures;

		BlockFaceTextures dirtTextures;
		dirtTextures.Top = dirtTextures.Bottom = dirtTextures.Front = dirtTextures.Back = dirtTextures.Left = dirtTextures.Right =
			SubTexture2D::CreateFromCoords(s_AtlasTexture, { 1, 0 }, spriteSize, cellSize);

		s_BlockTextures[BlockId::DIRT] = dirtTextures;

		BlockFaceTextures stoneTextures;
		stoneTextures.Top = stoneTextures.Bottom = stoneTextures.Front = stoneTextures.Back = stoneTextures.Left = stoneTextures.Right =
			SubTexture2D::CreateFromCoords(s_AtlasTexture, { 3, 0 }, spriteSize, cellSize);

		s_BlockTextures[BlockId::STONE] = stoneTextures;
	}

	const BlockFaceTextures& Blocks::GetTextures(BlockId id) {
		auto it = s_BlockTextures.find(id);
		if (it != s_BlockTextures.end()) {
			return it->second;
		}

		static BlockFaceTextures emptyTextures;
		return emptyTextures;
	}

#pragma endregion

#pragma region Block Class

	Block::Block()
	{
		m_Position = { 0, 0, 0 };
		m_Rotation = { 0, 90, 0 };
	}

#pragma endregion
}