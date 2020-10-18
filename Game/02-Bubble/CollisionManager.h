#pragma once
#include <string>
#include <glm/glm.hpp>
#include <map>
#include "Types.h"
#include "Brick.h"
#include <functional>



namespace game
{
namespace gameplay
{
namespace physics
{

enum class CollisionResult
{
	NoCollision,
	CollidedWithStaticBlock,
	CollidedWithDownScreen,
	CollidedWithBrick
};

class CollisionManager
{
public:
	CollisionManager(const std::string& i_staticCollisionMap, const uint32_t i_tileSize, const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks, std::function<void()> i_moveDown, std::function<void()> i_moveUp);

	CollisionResult CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size);
	CollisionResult CollisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size);
	CollisionResult CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY);
	CollisionResult CollisionMoveUp(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY);

private:
	void ProcessBlockCollision(uint32_t i_x, uint32_t i_y);
	void SetUpStaticCollisions(const std::string& i_staticCollisionMapPath, const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks);

	uint32_t m_currentMap = 2;
	std::vector<Matrix<std::string>> m_staticCollisions;
	std::vector<std::map<uint32_t,std::shared_ptr<Brick>>> m_bricks;
	std::function<void()> m_moveUpFunction;
	std::function<void()> m_moveDownFunction;
	const uint32_t m_tileSize;
};
}
}
}