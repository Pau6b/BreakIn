#pragma once
#include <memory>
#include "Sprite.h"
#include "CollisionManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


namespace game
{
class Player
{
public:
	Player(physics::CollisionManager& i_collisionsManager);
	void init(const glm::ivec2& i_tileMapPos, ShaderProgram& i_shaderProgram);
	void update(int i_deltaTime);
	void render();

	void setPosition(const glm::vec2& i_pos);
	
private:
	bool m_bJumping;
	glm::ivec2 m_tileMapDispl, m_posPlayer;
	int m_jumpAngle, m_startY;
	std::unique_ptr<Sprite> m_sprite;
	physics::CollisionManager& m_map;

};
}

