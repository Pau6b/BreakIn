#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <set>
#include <unordered_set>
#include "Scene.h"
#include "ShaderProgram.h"
#include <map>
#include <thread>

namespace game
{
namespace gameplay
{
	class Brick;
	class Player;
	class Coin;
	class BreakableBlock;
	class Ball;
	class Watcher;
	class Sensor;
	class Portal;
}
}

namespace game
{
namespace visuals
{
	class TileMap;
	class Sprite;
}
}

namespace game
{
	namespace gui
	{
		class Text;
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
	
	struct LevelResult
	{
		uint32_t currentLives;
		uint32_t currentPoints;
		uint32_t currentCoins;
	};

	LevelScene(const std::string& i_visualTilemapPath,
			   const std::string& i_physicsMapPath,
		       core::CheatSystem& i_cheatSystem,
			   sound::SoundSystem& i_soundSystem,
			   uint32_t i_currentMine);

	LevelScene(const std::string& i_visualTilemapPath,
		const std::string& i_physicsMapPath,
		core::CheatSystem& i_cheatSystem,
		sound::SoundSystem& i_soundSystem,
		uint32_t i_currentMine,
		const LevelResult& i_previousResult);

	~LevelScene();
	void init() override;
	void update(int i_deltaTime) override;
	void render() override;
	std::pair<core::Scene::SceneResult, uint32_t> GetSceneResult() override;
	void MoveLevelUp();
	void MoveLevelDown();

	LevelResult GetLevelResult();

private:
	void ParseBricks(std::string i_path);
	void OnBreakableBlockBroken(std::shared_ptr<BreakableBlock> i_brokenBlock);
	void Reset();
	void LoseHP();

	std::unique_ptr<visuals::ShaderProgram> m_texProgram;
	std::vector<std::vector<std::shared_ptr<Brick>>> m_bricks;
	std::vector<std::vector<std::shared_ptr<Coin>>> m_coins;
	std::map<uint32_t,std::shared_ptr<BreakableBlock>> m_keys;
	std::vector<std::shared_ptr<BreakableBlock>> m_blocksToCheck;
	std::map<uint32_t, std::shared_ptr<Sensor>> m_sensor;
	std::unique_ptr<visuals::TileMap> m_map;
	std::unique_ptr<physics::CollisionManager> m_collisionManager;
	std::vector<Portal*> m_portals;
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
	std::pair<core::Scene::SceneResult, glm::uint32_t> m_currentSceneResult = { core::Scene::SceneResult::NotFinished, 0 };
	core::CheatSystem& m_cheatSystem;
	uint32_t m_currentLives = 3;
	std::unique_ptr<gui::Text> m_text;
	uint32_t m_currentMine;
	std::unique_ptr<visuals::Sprite> m_mask;
};
}
}
