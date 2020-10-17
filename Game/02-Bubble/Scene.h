#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <set>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "CollisionManager.h"
#include "Brick.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

namespace game
{
class Scene
{

public:

	void init();
	void update(int i_deltaTime);
	void render();

private:
	void initShaders();
	void ParseBricks(std::string i_path);
	std::unique_ptr<ShaderProgram> m_texProgram;
	std::vector<std::vector<std::shared_ptr<Brick>>> m_bricks;
	std::unique_ptr<TileMap> m_map;
	std::unique_ptr<physics::CollisionManager> m_collisionManager;
	std::unique_ptr<Player> m_player;
	float m_currentTime;
	glm::mat4 m_projection;
	uint32_t m_currentMap = 2;
};
}

