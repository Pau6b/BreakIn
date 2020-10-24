#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"



namespace game
{
	namespace gameplay
	{

		Ball::Ball(physics::CollisionManager& i_collisionsManager)
			: m_map(i_collisionsManager)
			, m_size(12),
			m_dirBall(glm::vec2(0,1)),
			m_speed(2)
		{
			
			
		}

		void Ball::init(const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram)
		{
			m_sprite = std::make_unique<visuals::Sprite>(glm::ivec2(m_size, m_size), glm::vec2(1.0, 1.0), "images/BolaNieve.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
			m_tileMapDispl = i_tileMapPos;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posBall.x), float(m_tileMapDispl.y + m_posBall.y)));
		}

		void Ball::update(int i_deltaTime)
		{
			m_sprite->update(i_deltaTime);
			m_map.CollisionBall(m_posBall, m_dirBall, m_size, m_speed);
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posBall.x), float(m_tileMapDispl.y + m_posBall.y)));
		}

		void Ball::render()
		{
			m_sprite->render();
		}

		void Ball::setPosition(const glm::vec2& i_pos)
		{
			m_posBall = i_pos;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posBall.x), float(m_tileMapDispl.y + m_posBall.y)));
		}
	}
}