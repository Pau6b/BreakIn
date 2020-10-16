#include <cmath>
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
//#pau_todo This should be an enum class
enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


Player::Player(physics::CollisionManager& i_collisionsManager)
	: m_map(i_collisionsManager)
{

}

void Player::init(const glm::ivec2& i_tileMapPos, ShaderProgram& i_shaderProgram)
{
	m_bJumping = false;
	m_sprite = std::make_unique<Sprite>(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), "images/bub.png", PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
	m_sprite->setNumberAnimations(4);
	
	m_sprite->setAnimationSpeed(STAND_LEFT, 8);
	m_sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
	m_sprite->setAnimationSpeed(STAND_RIGHT, 8);
	m_sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
	m_sprite->setAnimationSpeed(MOVE_LEFT, 8);
	m_sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	m_sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	m_sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
	m_sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	m_sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	m_sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
	m_sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	m_sprite->changeAnimation(0);
	m_tileMapDispl = i_tileMapPos;
	m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posPlayer.x), float(m_tileMapDispl.y + m_posPlayer.y)));	
}

void Player::update(int i_deltaTime)
{
	m_sprite->update(i_deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(m_sprite->animation() != MOVE_LEFT)
			m_sprite->changeAnimation(MOVE_LEFT);
		m_posPlayer.x -= 2;
		if(m_map.CollisionMoveLeft(m_posPlayer, glm::ivec2(32, 32)) == physics::CollisionResult::CollidedWithStaticBlock)
		{
			m_posPlayer.x += 2;
			m_sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(m_sprite->animation() != MOVE_RIGHT)
			m_sprite->changeAnimation(MOVE_RIGHT);
		m_posPlayer.x += 2;
		if(m_map.CollisionMoveRight(m_posPlayer, glm::ivec2(32, 32)) == physics::CollisionResult::CollidedWithStaticBlock)
		{
			m_posPlayer.x -= 2;
			m_sprite->changeAnimation(STAND_RIGHT);
		}
	}

	else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		m_posPlayer.y -= 2;
		if (m_map.CollisionMoveUp(m_posPlayer, glm::ivec2(32, 32), &m_posPlayer.y) != physics::CollisionResult::CollidedWithStaticBlock)
		{
			//m_posPlayer.y -= 2;
		}
	}

	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		m_posPlayer.y += 2;
		if (m_map.CollisionMoveDown(m_posPlayer, glm::ivec2(32, 32), &m_posPlayer.y) != physics::CollisionResult::CollidedWithStaticBlock)
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
	
	
	m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posPlayer.x), float(m_tileMapDispl.y + m_posPlayer.y)));
}

void Player::render()
{
	m_sprite->render();
}

void Player::setPosition(const glm::vec2& i_pos)
{
	m_posPlayer = i_pos;
	m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posPlayer.x), float(m_tileMapDispl.y + m_posPlayer.y)));
}

}


