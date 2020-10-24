#pragma once
#include <string>
#include <glm/glm.hpp>
#include <map>
#include "Types.h"
#include "Brick.h"
#include <functional>
#include <tuple>


namespace game { namespace gameplay { class Player; } }




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
	CollidedWithScreen,
	CollidedWithBrick,
	CollidedWithBarLeft,
	CollidedWithBarRight,
	CollidedWithPlayer
};

class CollisionManager
{
public:
	CollisionManager(const std::string& i_staticCollisionMap, const uint32_t i_tileSize, const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks, std::function<void()> i_moveDown, std::function<void()> i_moveUp);

	CollisionResult CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size);
	CollisionResult CollisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size);
	CollisionResult CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY);
	CollisionResult CollisionMoveUp(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY);
	CollisionResult CollisionBall(glm::ivec2& i_pos, glm::vec2& i_dir, const int& i_size, const int& i_speed);
	void LinkPlayer(Player* i_player);

private:
	void ProcessBlockCollision(uint32_t i_x, uint32_t i_y);
	void SetUpStaticCollisions(const std::string& i_staticCollisionMapPath, const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks);
	CollisionResult CheckCollision(const int& i_posX, const int& i_posY);
	std::tuple<uint32_t, uint32_t, uint32_t> CheckDirectionOfCollision(const int& i_XposMid, const int& i_YposMid, const int& i_XposRight, const int& i_XposLeft, const int& i_YposUp, const int& i_YposDown);
	bool CollisionPlayer(int i_Ydown, int i_Xmid, int i_Xleft, int i_Xright, int i_dirY, const int& i_Speed);
	uint32_t m_currentMap = 2;
	std::vector<Matrix<std::string>> m_staticCollisions;
	std::vector<std::map<uint32_t,std::shared_ptr<Brick>>> m_bricks;
	std::function<void()> m_cameraMoveUpFunction;
	std::function<void()> m_cameraMoveDownFunction;
	const uint32_t m_tileSize;
	Player *m_player;
};
}
}
}