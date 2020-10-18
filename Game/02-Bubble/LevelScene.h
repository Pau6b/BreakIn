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

	void init() override;
	void update(int i_deltaTime) override;
	void render() override;

private:
	void initShaders();
	std::unique_ptr<TileMap> m_map;
	std::unique_ptr<physics::CollisionManager> m_collisionManager;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<visuals::ShaderProgram> m_texProgram;
	float m_currentTime;
	glm::mat4 m_projection;

};
}
}

