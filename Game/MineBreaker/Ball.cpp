#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"
#include "Player.h"
#include "CollisionManager.h"



namespace game
{
	namespace gameplay
	{
		namespace 
		{
			glm::vec2 CreateRandomStartDirection()
			{
				uint32_t range = 1;
				float x = float(rand()%(range*1000))/1000.f-range/2.f;
				return glm::normalize(glm::vec2(x, -1));
			}
		}

		Ball::Ball(physics::CollisionManager& i_collisionsManager, const Player& i_player, uint32_t i_currentMap, const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram, float i_maxSpeed)
			: m_map(i_collisionsManager)
			, m_size(12)
			, m_state(BallState::FollowingPlayer)
			, m_dirBall(CreateRandomStartDirection())
			, m_speed(3.f)
			, m_player(i_player)
			, m_currentMap(i_currentMap)
			, m_tileMapDispl(i_tileMapPos)
			, m_maxSpeed(i_maxSpeed)
		{
			m_sprite = std::make_unique<visuals::Sprite>(glm::ivec2(m_size, m_size), glm::vec2(1.0, 1.0), "images/BolaNieve.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posBall.x), float(m_tileMapDispl.y + m_posBall.y)));
		}


		void Ball::Update(int i_deltaTime)
		{
			m_sprite->update(i_deltaTime);
			if (m_state == BallState::Free)
			{
				physics::CollisionResult collisionResult = m_map.CollisionBall(m_posBall, m_dirBall, m_size, m_speed, this);
				SetPosition(m_posBall);
			}
			else if (m_state == BallState::FollowingPlayer)
			{
				if (core::Game::instance().getSpecialKey(GLUT_KEY_UP) || core::Game::instance().getSpecialKey(GLUT_KEY_DOWN))
				{
					m_state = BallState::Free;
					Update(i_deltaTime);
				}
				else
				{
					m_currentTimeElapsed += i_deltaTime;
					if (m_currentTimeElapsed < k_timeToStartMoving)
					{
						m_posBall = m_player.GetPosition();
						m_posBall.y -= m_size+2;
						m_posBall.x += m_player.GetSize().x/2 - m_size/2;
						SetPosition(m_posBall);
					}
					else
					{
						m_state = BallState::Free;
						Update(i_deltaTime);
					}
				}
			}
		}

		void Ball::Render()
		{
			m_sprite->render();
		}

		void Ball::SetInPortal(bool i_inPortal)
		{
			m_inPortal = i_inPortal;
		}

		bool Ball::IsInPortal() const
		{
			return m_inPortal;
		}

		void Ball::SetPosition(const glm::vec2& i_pos)
		{
			m_posBall = i_pos;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posBall.x), float(m_tileMapDispl.y + m_posBall.y)));
		}

		void Ball::Reset()
		{
			m_currentTimeElapsed = 0;
			m_state = BallState::FollowingPlayer;
			m_dirBall = CreateRandomStartDirection();
		}

	}
}