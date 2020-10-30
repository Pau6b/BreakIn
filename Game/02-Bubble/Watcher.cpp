#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Watcher.h"
#include "Game.h"
#include "Player.h"



namespace game
{
	namespace gameplay
	{

		Watcher::Watcher(const Player& i_player)
			: m_size(12)
			, m_state(WatcherState::FollowingPlayer)
			, m_dirWatcher(glm::vec2(0, 1))
			, m_speed(2.5f)
			, m_player(i_player)
		{

		}

		void Watcher::Init(const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram)
		{
			m_sprite = std::make_unique<visuals::Sprite>(glm::ivec2(m_size, m_size), glm::vec2(1.0, 1.0), "images/bat.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
			m_tileMapDispl = i_tileMapPos;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posWatcher.x), float(m_tileMapDispl.y + m_posWatcher.y)));
		}

		void Watcher::Update(int i_deltaTime)
		{
			m_sprite->update(i_deltaTime);
			if (m_state == WatcherState::Quiet)
			{
				//m_map.CollisionBall(m_posWatcher, m_dirWatcher, m_size, m_speed);
				SetPosition(m_posWatcher);
			}
			else if (m_state == WatcherState::FollowingPlayer)
			{
				if (core::Game::instance().getSpecialKey(GLUT_KEY_UP) || core::Game::instance().getSpecialKey(GLUT_KEY_DOWN))
				{
					m_state = WatcherState::Quiet;
					Update(i_deltaTime);
				}
				else
				{
					m_currentTimeElapsed += i_deltaTime;
					if (m_currentTimeElapsed < k_timeToStartMoving)
					{
						m_posWatcher = m_player.GetPosition();
						m_posWatcher.y -= m_size + 2;
						m_posWatcher.x += m_player.GetSize().x / 2 - m_size / 2;
						SetPosition(m_posWatcher);
					}
					else
					{
						m_state = WatcherState::Quiet;
						Update(i_deltaTime);
					}
				}
			}
		}

		void Watcher::Render()
		{
			m_sprite->render();
		}

		void Watcher::SetPosition(const glm::vec2& i_pos)
		{
			m_posWatcher = i_pos;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posWatcher.x), float(m_tileMapDispl.y + m_posWatcher.y)));
		}

		void Watcher::Reset()
		{
			m_state = WatcherState::FollowingPlayer;
		}

	}
}