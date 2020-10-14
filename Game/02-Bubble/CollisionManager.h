#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Types.h"



namespace game
{
namespace physics
{

enum class CollisionResult
{
	NoCollision,
	CollidedWithStaticBlock
};

class CollisionManager
{
public:
	CollisionManager(const std::string& i_staticCollisionMap, const uint32_t i_tileSize);

	CollisionResult CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size) const;
	CollisionResult CollisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size) const;
	CollisionResult CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY) const;

private:
	void SetUpStaticCollisions(const std::string& i_staticCollisionMapPath);
	
	uint32_t m_currentMap = 2;
	std::vector<Matrix<char>>m_staticCollisions;
	const uint32_t m_tileSize;
};
}
}