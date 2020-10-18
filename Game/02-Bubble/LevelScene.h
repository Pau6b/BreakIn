#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <set>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "CollisionManager.h"
#include "Brick.h"
#include "Scene.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

namespace game
{
namespace gameplay
{
class LevelScene : public core::Scene
{
public:
	LevelScene(const std::string& i_visualTilemapPath, const std::string& i_physicsMapPath);
	void init() override;
	void update(int i_deltaTime) override;
	void render() override;
	std::pair<core::Scene::SceneResult, uint32_t> GetSceneResult() override;

private:
	void ParseBricks(std::string i_path);

	std::unique_ptr<visuals::ShaderProgram> m_texProgram;
	std::vector<std::vector<std::shared_ptr<Brick>>> m_bricks;
	std::unique_ptr<TileMap> m_map;
	std::unique_ptr<physics::CollisionManager> m_collisionManager;
	std::unique_ptr<Player> m_player;
	float m_currentTime;
	glm::mat4 m_projection;
	uint32_t m_currentMap = 2;
	std::string m_visualTilemapPath;
	std::string m_physicsMapPath;
};
}
}

