#pragma once
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <tuple>
#include "Types.h"
#include <utility>

namespace game
{
namespace core
{
	class CheatSystem;
}
}

namespace game
{
namespace gameplay
{
	class Brick;
	class BreakableBlock;
	class Coin;
	class Player;
	class Sensor;
}
}

namespace game
{
namespace sound
{
	class SoundSystem;
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
	CollidedWithScreen,
	CollidedWithBrick,
	CollidedWithBarLeft,
	CollidedWithBarRight,
	CollidedWithPlayer,
	CollidedWithAlarm
};

class CollisionManager
{
public:
	CollisionManager(const std::string& i_staticCollisionMap,
					 const uint32_t i_tileSize,
					 uint32_t i_currentMap,
					 const std::vector<std::unordered_set<std::shared_ptr<Brick>>>& i_bricks,
					 const std::vector<std::unordered_set<std::shared_ptr<Coin>>>& i_coins,
					 const std::map<uint32_t, std::shared_ptr<BreakableBlock>>& i_keys,
					 const std::map<uint32_t, std::shared_ptr<Sensor>>& i_sensor,
					 std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> i_onBrokenBlockFunction,
					 std::function<void()> i_moveDown,
					 std::function<void()> i_moveUp,
					 const core::CheatSystem& i_cheatSystem,
					 sound::SoundSystem& i_soundSystem);

	CollisionResult CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size);
	CollisionResult CollisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size);
	CollisionResult CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY);
	CollisionResult CollisionMoveUp(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY);
	CollisionResult CollisionBall(glm::vec2& i_pos, glm::vec2& i_dir, const int& i_size, const float& i_speed);
	void LinkPlayer(Player* i_player);
	void SetCurrentMap(uint32_t i_currentMap);
	std::pair<uint32_t,uint32_t> WipeDoorPositions();

private:
	//#pau_todo i know this won't be fixed, but it is the result of a disastrous collision manager
	void PlayBreakableBlockSound( std::shared_ptr<BreakableBlock> i_breakableBlock );

	void ProcessBlockCollision(uint32_t i_x, uint32_t i_y);
	void SetUpStaticCollisions(const std::string& i_staticCollisionMapPath,
							   const std::vector<std::unordered_set<std::shared_ptr<Brick>>>& i_bricks,
							   const std::vector<std::unordered_set<std::shared_ptr<Coin>>>& i_coins,
							   const std::map<uint32_t,std::shared_ptr<BreakableBlock>>& i_keys);
	CollisionResult CheckCollision(const int& i_posX, const int& i_posY);
	std::tuple<uint32_t, uint32_t, uint32_t> CheckDirectionOfCollision(const int& i_XposMid, const int& i_YposMid, const int& i_XposRight, const int& i_XposLeft, const int& i_YposUp, const int& i_YposDown);
	bool CollisionPlayer(const glm::vec2& i_pos, uint32_t i_size, float i_dirY, const int32_t& i_Speed);
	uint32_t m_currentMap;
	std::vector<Matrix<std::string>> m_staticCollisions;
	std::vector<std::map<uint32_t,std::shared_ptr<BreakableBlock>>> m_breakableBlocks;
	std::map<uint32_t, std::shared_ptr<Sensor>> m_sensor;
	std::function<void()> m_cameraMoveUpFunction;
	std::function<void()> m_cameraMoveDownFunction;
	std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> m_onBreakableBlockBroken;
	const uint32_t m_tileSize;
	uint32_t m_mapSizeY;
	Player* m_player;
	uint32_t m_coins, m_points;
	const core::CheatSystem& m_cheatSystem;
	sound::SoundSystem& m_soundSystem;
};
}
}
}
