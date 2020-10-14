#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "CollisionManager.h"


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

private:
	std::unique_ptr<TileMap> m_map;
	std::unique_ptr<physics::CollisionManager> m_collisionManager;
	std::unique_ptr<Player> m_player;
	ShaderProgram m_texProgram;
	float m_currentTime;
	glm::mat4 m_projection;

};
}

