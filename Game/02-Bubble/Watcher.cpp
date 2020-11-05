#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Watcher.h"
#include "Game.h"
#include "Player.h"

enum Anims
{
	FLY, RESTING
};

#define WATCHER_INITIAL_POS_X 1
#define WATCHER_INITIAL_POS_Y 1

namespace game
{
	namespace gameplay
	{

		Watcher::Watcher(const Player& i_player, const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram, std::function<void()> i_LoseHP)
			: m_size(32)
			, m_state(WatcherState::Quiet)
			, m_dirWatcher(glm::vec2(0, 1))
			, m_speed(2.5f)
			, m_player(i_player)
			, m_LoseHp(i_LoseHP)
		{
			m_sprite = std::make_unique<visuals::Sprite>(glm::ivec2(m_size, m_size), glm::vec2(1/7.f, 1.0), "images/bat.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
			m_sprite->setNumberAnimations(2);
			m_sprite->setAnimationSpeed(FLY, 8);
			m_sprite->addKeyframe(FLY, glm::vec2(0/7.f, 0.f));
			m_sprite->addKeyframe(FLY, glm::vec2(1/7.f, 0.f));
			m_sprite->addKeyframe(FLY, glm::vec2(2/7.f, 0.f));
			m_sprite->addKeyframe(FLY, glm::vec2(3/7.f, 0.f));
			m_sprite->setAnimationSpeed(RESTING, 1);
			m_sprite->addKeyframe(RESTING, glm::vec2(4/7.f, 0.f));
			m_sprite->addKeyframe(RESTING, glm::vec2(5/7.f, 0.f));
			m_sprite->addKeyframe(RESTING, glm::vec2(6/7.f, 0.f));
			m_sprite->changeAnimation(RESTING);
			m_tileMapDispl = i_tileMapPos;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posWatcher.x), float(m_tileMapDispl.y + m_posWatcher.y)));
		}

		void Watcher::Update(int i_deltaTime)
		{
			m_sprite->update(i_deltaTime);
			if (m_state == WatcherState::FollowingPlayer)
			{
				
			}
		}

		void Watcher::Render()
		{
			m_sprite->render();
		}

		Watcher::ResultMovement Watcher::FollowPlayer()
		{
			glm::ivec2 pos = m_player.GetPosition();
			if (pos.x -1 <= (m_posWatcher.x) && (pos.x + 33) >= (m_posWatcher.x + m_size/2)
				&& pos.y - 12  <= (m_posWatcher.y + m_size/2) && pos.y + 24 >= (m_posWatcher.y + m_size/2) )
			{
				m_LoseHp();
				return ResultMovement::PlayerHit;
			}
			if (m_state == WatcherState::Quiet)
			{
				m_state = WatcherState::FollowingPlayer;
				m_sprite->changeAnimation(FLY);
				glm::ivec2 pos = m_player.GetPosition();
				m_dirWatcher = glm::ivec2(pos.x - m_posWatcher.x, pos.y - m_posWatcher.y);
				float tot_square_rot = std::sqrt(m_dirWatcher.x*m_dirWatcher.x + m_dirWatcher.y*m_dirWatcher.y);
				if (tot_square_rot) 
				{
					m_dirWatcher.x /= tot_square_rot;
					m_dirWatcher.y /= tot_square_rot;
				}
				m_destPos = pos;
			}
			else if (m_state == WatcherState::FollowingPlayer)
			{
				m_posWatcher = glm::vec2(m_posWatcher.x + m_dirWatcher.x, m_posWatcher.y + m_dirWatcher.y);
				m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posWatcher.x), float(m_tileMapDispl.y + m_posWatcher.y)));
				if (signbit(m_destPos.x - m_posWatcher.x) != signbit(m_dirWatcher.x) || signbit(m_destPos.y - m_posWatcher.y)  != signbit(m_dirWatcher.y))
				{
					m_state = WatcherState::LocatingPlayer;
				}
			}
			else if (m_state == WatcherState::LocatingPlayer)
			{
				m_dirWatcher = glm::ivec2(pos.x - m_posWatcher.x, pos.y - m_posWatcher.y);
				float tot_square_rot = std::sqrt(m_dirWatcher.x*m_dirWatcher.x + m_dirWatcher.y*m_dirWatcher.y);
				if (tot_square_rot > 0)
				{
					m_dirWatcher.x /= tot_square_rot;
					m_dirWatcher.y /= tot_square_rot;
				}
				m_destPos = pos;
				m_state = WatcherState::FollowingPlayer;
			}
			return ResultMovement::None;
		}

		void Watcher::SetPosition(const glm::vec2& i_pos)
		{
			m_posWatcher = i_pos;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posWatcher.x), float(m_tileMapDispl.y + m_posWatcher.y)));
		}

		void Watcher::Reset()
		{
			m_state = WatcherState::Quiet;
			m_posWatcher = glm::vec2(WATCHER_INITIAL_POS_X*16, WATCHER_INITIAL_POS_Y*16);
			m_sprite->changeAnimation(RESTING);
			m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posWatcher.x), float(m_tileMapDispl.y + m_posWatcher.y)));
		}

	}
}