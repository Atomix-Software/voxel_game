#pragma once

#include <Arcane/Core/Base.h>
#include <Arcane/Render/SubTexture.h>

#include <glm/vec3.hpp>

namespace Game
{
	struct BlockFaceTextures {
		Arcane::Shared<Arcane::SubTexture2D> Top;
		Arcane::Shared<Arcane::SubTexture2D> Bottom;
		Arcane::Shared<Arcane::SubTexture2D> Front;
		Arcane::Shared<Arcane::SubTexture2D> Back;
		Arcane::Shared<Arcane::SubTexture2D> Left;
		Arcane::Shared<Arcane::SubTexture2D> Right;
	};

	enum BlockId : uint32_t
	{
		AIR = 0,
		GRASS, DIRT,
		STONE, SAND,
		LOG, WOOD_PLANK,

		SIZE
	};

	enum Face : uint32_t
	{
		Front = 0, Right,
		Back, Left,
		Top, Bottom
	};
	
	struct FaceUV {
		glm::vec2 Offset = glm::vec2(0.0f);
		glm::vec2 Scale  = glm::vec2(1.0f);
	}; 
	
	struct BlockUVData {
		FaceUV Faces[6]; // 6 faces per block
	};

	class Blocks
	{
	public:

	public:
		static void Init();

		static const glm::vec2* GetFace(BlockId id, Face face);
		static const BlockFaceTextures& GetTextures(BlockId id);
		inline static const Arcane::Shared<Arcane::Texture2D>& GetAtlas() { return s_AtlasTexture; }

		static std::vector<FaceUV> GenerateSSBOData();

	private:
		static std::unordered_map<BlockId, std::array<Arcane::Shared<Arcane::SubTexture2D>, 6>> s_BlockFaces;
		static std::unordered_map<BlockId, BlockFaceTextures> s_BlockTextures;
		static Arcane::Shared<Arcane::Texture2D> s_AtlasTexture;
	};

	class Block
	{
	public:
		Block();
		~Block() {}

		inline glm::vec3& GetPosition() { return m_Position; }
		inline const glm::vec3& GetPosition() const { return m_Position; }

		inline glm::vec3& GetRotation() { return m_Rotation; }
		inline const glm::vec3& GetRotation() const { return m_Rotation; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

	};
}