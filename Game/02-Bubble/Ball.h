#pragma once
#include <memory>
#include "Sprite.h"
#include "CollisionManager.h"

namespace game
{
	namespace gameplay
	{
		class Player;
	}
}


// Ball is a sprite


namespace game
{
namespace gameplay
{
class Ball
{
public:
	Ball(physics::CollisionManager& i_collisionsManager, const Player& i_player);
	void Init(const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram);
	void Update(int i_deltaTime);
	void Render();

	void SetPosition(const glm::vec2& i_pos);
	void Reset();

private:

	enum class BallState
	{
		FollowingPlayer,
		Free
	} m_state;

	glm::vec2 m_tileMapDispl, m_posBall;
	glm::vec2  m_dirBall;
	std::unique_ptr<visuals::Sprite> m_sprite;
	const Player& m_player;
	physics::CollisionManager& m_map;
	int m_size;
	float m_speed;
	const uint32_t k_timeToStartMoving = 30000;
	uint32_t m_currentTimeElapsed = 0;
};
}
}

