#include "Block.h"

namespace Game
{
	using namespace Arcane;

#pragma region Blocks Database Class

	std::unordered_map<BlockId, std::array<Shared<SubTexture2D>, 6>> Blocks::s_BlockFaces;
	std::unordered_map<BlockId, BlockFaceTextures> Blocks::s_BlockTextures;
	Shared<Texture2D> Blocks::s_AtlasTexture;

	void Blocks::Init() {
		s_AtlasTexture = Texture2D::Create("assets/textures/blocks.png");

		const glm::vec2 cellSize = { 1.0f, 1.0f };
		const glm::vec2 spriteSize = { 16.0f, 16.0f };

		BlockFaceTextures air;
		air.Top = air.Bottom = air.Front = air.Back = air.Left = air.Right =
			SubTexture2D::CreateFromCoords(s_AtlasTexture, { 0, 0 }, { 0, 0 }, { 0, 0 });

		s_BlockFaces[BlockId::AIR] = {
			air.Front,
			air.Right,
			air.Back,
			air.Left,
			air.Top,
			air.Bottom
		};

		s_BlockTextures[BlockId::AIR] = air;

		BlockFaceTextures grassTextures;
		grassTextures.Top = SubTexture2D::CreateFromCoords(s_AtlasTexture,    { 0, 15 }, spriteSize, cellSize);
		grassTextures.Bottom = SubTexture2D::CreateFromCoords(s_AtlasTexture, { 2, 15 }, spriteSize, cellSize);
		grassTextures.Front = SubTexture2D::CreateFromCoords(s_AtlasTexture,  { 1, 15 }, spriteSize, cellSize);
		grassTextures.Back = grassTextures.Front;
		grassTextures.Left = grassTextures.Front;
		grassTextures.Right = grassTextures.Front;

		s_BlockFaces[BlockId::GRASS] = {
			grassTextures.Front,
			grassTextures.Right,
			grassTextures.Back,
			grassTextures.Left,
			grassTextures.Top,
			grassTextures.Bottom
		};

		s_BlockTextures[BlockId::GRASS] = grassTextures;

		BlockFaceTextures dirtTextures;
		dirtTextures.Top = dirtTextures.Bottom = dirtTextures.Front = dirtTextures.Back = dirtTextures.Left = dirtTextures.Right =
			SubTexture2D::CreateFromCoords(s_AtlasTexture, { 2, 15 }, spriteSize, cellSize);

		s_BlockFaces[BlockId::DIRT] = {
			dirtTextures.Front,
			dirtTextures.Right,
			dirtTextures.Back,
			dirtTextures.Left,
			dirtTextures.Top,
			dirtTextures.Bottom
		};

		s_BlockTextures[BlockId::DIRT] = dirtTextures;

		BlockFaceTextures stoneTextures;
		stoneTextures.Top = stoneTextures.Bottom = stoneTextures.Front = stoneTextures.Back = stoneTextures.Left = stoneTextures.Right =
			SubTexture2D::CreateFromCoords(s_AtlasTexture, { 3, 15 }, spriteSize, cellSize);

		s_BlockFaces[BlockId::STONE] = {
			stoneTextures.Front,
			stoneTextures.Right,
			stoneTextures.Back,
			stoneTextures.Left,
			stoneTextures.Top,
			stoneTextures.Bottom
		};

		s_BlockTextures[BlockId::STONE] = stoneTextures;
	}

	const glm::vec2* Blocks::GetFace(BlockId id, Face face)
	{
		const BlockFaceTextures& textures = Blocks::GetTextures(id);

		switch (face)
		{
			case Face::Front:  return textures.Front  ? textures.Front->GetTexCoords()  : nullptr;
			case Face::Right:  return textures.Right  ? textures.Right->GetTexCoords()  : nullptr;
			case Face::Back:   return textures.Back   ? textures.Back->GetTexCoords()   : nullptr;
			case Face::Left:   return textures.Left   ? textures.Left->GetTexCoords()   : nullptr;
			case Face::Top:    return textures.Top    ? textures.Top->GetTexCoords()    : nullptr;
			case Face::Bottom: return textures.Bottom ? textures.Bottom->GetTexCoords() : nullptr;
			default:           return nullptr;
		}
	}

	const BlockFaceTextures& Blocks::GetTextures(BlockId id) 
	{
		auto it = s_BlockTextures.find(id);
		if (it != s_BlockTextures.end()) {
			return it->second;
		}

		static BlockFaceTextures emptyTextures;
		return emptyTextures;
	}

	std::vector<FaceUV> Blocks::GenerateSSBOData()
	{
		std::vector<FaceUV> result;

		auto extract = [](const Shared<SubTexture2D>& sub) -> FaceUV {
			if (!sub) return { {0.0f, 0.0f}, {1.0f, 1.0f} };
			return { sub->GetTexCoords()[0], sub->GetTexCoords()[2] - sub->GetTexCoords()[0] };
		};

		for (const auto& [id, textures] : s_BlockTextures) {
			BlockFaceTextures block_textures = GetTextures(id);

			result.push_back(extract(block_textures.Front));
			result.push_back(extract(block_textures.Right));
			result.push_back(extract(block_textures.Back));
			result.push_back(extract(block_textures.Left));
			result.push_back(extract(block_textures.Top));
			result.push_back(extract(block_textures.Bottom));
		}

		return result;
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