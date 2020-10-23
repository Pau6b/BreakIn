#pragma once
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <unordered_set>
#include <memory>
#include <functional>
#include "Types.h"

namespace game
{
namespace gameplay
{
	class Brick;
	class BreakableBlock;
	class Coin;
}
}

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
	CollisionManager(const std::string& i_staticCollisionMap,
					 const uint32_t i_tileSize,
					 const std::vector<std::unordered_set<std::shared_ptr<Brick>>>& i_bricks,
					 const std::vector<std::unordered_set<std::shared_ptr<Coin>>>& i_coins,
					 std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> i_onBrokenBlockFunction);

	CollisionResult CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size);
	CollisionResult CollisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size);
	CollisionResult CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY);
	CollisionResult CollisionMoveUp(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY);

private:
	void ProcessBlockCollision(uint32_t i_x, uint32_t i_y);
	void SetUpStaticCollisions(const std::string& i_staticCollisionMapPath, const std::vector<std::unordered_set<std::shared_ptr<Brick>>>& i_bricks, const std::vector<std::unordered_set<std::shared_ptr<Coin>>>& i_coins);

	uint32_t m_currentMap = 2;
	std::vector<Matrix<std::string>> m_staticCollisions;
	std::vector<std::map<uint32_t,std::shared_ptr<BreakableBlock>>> m_breakableBlocks;

	std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> m_onBreakableBlockBroken;
	const uint32_t m_tileSize;
};
}
}
}