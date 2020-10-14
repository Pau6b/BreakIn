#pragma once
#include <string>
#include "Types.h"


namespace game
{
namespace physics
{
class CollisionManager
{
public:
	CollisionManager(const std::string& i_collisionMap, const uint32_t i_tileSize);
private:
	void SetUpStaticCollisions(const std::string& i_staticCollisionMapPath);
	
	std::vector<Matrix<char>>m_staticCollisions;
	const uint32_t m_tileSize;
};
}
}