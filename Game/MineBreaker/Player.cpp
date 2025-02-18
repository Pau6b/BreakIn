
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

namespace game
{
namespace gameplay
{
//#pau_todo This should be an enum class
enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


Player::Player(physics::CollisionManager& i_collisionsManager)
	: m_map(i_collisionsManager),
	m_sizePlayer(glm::ivec2(32,12))
{

}

void Player::Init(const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram, const glm::ivec2& i_startPos, uint32_t i_currentMap, uint32_t i_mapSizeY)
{
	m_bJumping = false;
	m_sprite = std::make_unique<visuals::Sprite>(glm::ivec2(32, 12), glm::vec2(1.0, 1.0), "images/slime.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
	m_tileMapDispl = i_tileMapPos;
	m_pos = i_startPos;
	m_startingPos = i_startPos;
	m_mapSizeY = i_mapSizeY;
	m_currentMap = i_currentMap;
	SetPosition(m_pos);
}

void Player::Update(int i_deltaTime)
{
	m_sprite->update(i_deltaTime);
	if(core::Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(m_sprite->animation() != MOVE_LEFT)
			m_sprite->changeAnimation(MOVE_LEFT);
		m_pos.x -= 4;
		if(m_map.CollisionMoveLeft(m_pos, glm::ivec2(32, 12)) == physics::CollisionResult::CollidedWithStaticBlock)
		{
			m_pos.x += 4;
			m_sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(core::Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(m_sprite->animation() != MOVE_RIGHT)
			m_sprite->changeAnimation(MOVE_RIGHT);
		m_pos.x += 4;
		if(m_map.CollisionMoveRight(m_pos, glm::ivec2(32, 12)) == physics::CollisionResult::CollidedWithStaticBlock)
		{
			m_pos.x -= 4;
			m_sprite->changeAnimation(STAND_RIGHT);
		}
	}

	else if (core::Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		m_pos.y -= 4;
		if (m_map.CollisionMoveUp(m_pos, glm::ivec2(32, 12), &m_pos.y) != physics::CollisionResult::CollidedWithStaticBlock)
		{
			//m_posPlayer.y -= 2;
		}
	}

	else if (core::Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		m_pos.y += 4;
		if (m_map.CollisionMoveDown(m_pos, glm::ivec2(32, 12), &m_pos.y) == physics::CollisionResult::NoCollision)
		{
			//m_posPlayer.y += 2;
		}
	}

	else
	{
		if(m_sprite->animation() == MOVE_LEFT)
			m_sprite->changeAnimation(STAND_LEFT);
		else if(m_sprite->animation() == MOVE_RIGHT)
			m_sprite->changeAnimation(STAND_RIGHT);
	}
	
	m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_pos.x), float(m_tileMapDispl.y + m_pos.y)));
}

void Player::Render()
{
	m_sprite->render();
}

void Player::SetPosition(const glm::vec2& i_pos)
{
	m_pos = i_pos;
	m_pos.y %= m_mapSizeY;
	m_pos.y += m_mapSizeY * (2 - m_currentMap);
	glm::vec2 position = glm::vec2(float(m_tileMapDispl.x + m_pos.x), float(m_tileMapDispl.y + m_pos.y));
	m_sprite->setPosition(position);
}

void Player::SetCurrentMap(uint32_t i_currentMap)
{
	m_currentMap = i_currentMap;
	SetPosition(m_pos);
}

glm::ivec2 Player::GetPosition() const
{
	return m_pos;
}

glm::ivec2 Player::GetSize() const
{
	return m_sizePlayer;
}

void Player::Reset()
{
	SetPosition(m_startingPos);
}

}
}