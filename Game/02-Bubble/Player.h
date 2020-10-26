#pragma once
#include <memory>
#include "Sprite.h"
#include "CollisionManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


namespace game
{
namespace gameplay
{
class Player
{
public:
	Player(physics::CollisionManager& i_collisionsManager);
	void Init(const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram, const glm::ivec2& i_startingPos);
	void Update(int i_deltaTime);
	void Render();
	void SetPosition(const glm::vec2& i_pos);
	glm::ivec2 GetPosition() const;
	glm::ivec2 GetSize() const;
	void Reset(uint32_t i_currentLevel, uint32_t i_levelQuantity, uint32_t i_levelSizeY);
	
private:
	bool m_bJumping;
	glm::ivec2 m_tileMapDispl;
	glm::ivec2 m_pos = glm::ivec2(-1,-1);
	glm::ivec2 m_startingPos;
	glm::ivec2 m_sizePlayer;
	int m_jumpAngle, m_startY;
	std::unique_ptr<visuals::Sprite> m_sprite;
	physics::CollisionManager& m_map;

};
}
}

