#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <set>
#include <unordered_set>
#include "Scene.h"
#include "ShaderProgram.h"

namespace game
{
namespace gameplay
{
	class Brick;
	class Player;
	class Coin;
	class BreakableBlock;
	class Ball;
}
}

namespace game
{
namespace visuals
{
	class TileMap;
}
}

namespace game
{
namespace gameplay
{
namespace physics
{
	class CollisionManager;
}
}
}

namespace game
{
namespace core
{
	class CheatSystem;
}
}

namespace game
{
namespace sound
{
	class SoundSystem;
}
}


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

namespace game
{
namespace gameplay
{
class LevelScene : public core::Scene
{
public:
	LevelScene(const std::string& i_visualTilemapPath, const std::string& i_physicsMapPath, const core::CheatSystem& i_cheatSystem, sound::SoundSystem& i_soundSystem);
	~LevelScene();
	void init() override;
	void update(int i_deltaTime) override;
	void render() override;
	std::pair<core::Scene::SceneResult, uint32_t> GetSceneResult() override;
	void MoveLevelUp();
	void MoveLevelDown();

private:
	void ParseBricks(std::string i_path);
	void OnBreakableBlockBroken(std::shared_ptr<BreakableBlock> i_brokenBlock);
	void Reset();

	std::unique_ptr<visuals::ShaderProgram> m_texProgram;
	std::vector<std::unordered_set<std::shared_ptr<Brick>>> m_bricks;
	std::vector<std::unordered_set<std::shared_ptr<Coin>>> m_coins;
	std::map<uint32_t,std::shared_ptr<BreakableBlock>> m_keys;
	std::vector<std::shared_ptr<BreakableBlock>> m_blocksToCheck;
	std::unique_ptr<visuals::TileMap> m_map;
	std::unique_ptr<physics::CollisionManager> m_collisionManager;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Ball> m_ball;
	sound::SoundSystem& m_soundSystem;
	float m_currentTime;
	glm::mat4 m_projection, m_traslation;
	uint32_t m_levelQuantity;
	uint32_t m_levelSizeY;
	uint32_t m_currentMap;
	float* m_projectionY;
	std::string m_visualTilemapPath;
	std::string m_physicsMapPath;
	core::Scene::SceneResult m_currentSceneResult = core::Scene::SceneResult::NotFinished;
	const core::CheatSystem& m_cheatSystem;
	uint32_t m_currentLives = 3;
};
}
}
