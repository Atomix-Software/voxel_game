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

	class Blocks
	{
	public:
		static void Init();
		static const BlockFaceTextures& GetTextures(BlockId id);

	private:
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