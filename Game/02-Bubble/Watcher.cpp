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
#define WATCHER_INITIAL_POS_Y 21

namespace game
{
	namespace gameplay
	{

		Watcher::Watcher(const Player& i_player, const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram)
			: m_size(32)
			, m_state(WatcherState::Quiet)
			, m_dirWatcher(glm::vec2(0, 1))
			, m_speed(2.5f)
			, m_player(i_player)
		{
			m_sprite = std::make_unique<visuals::Sprite>(glm::ivec2(m_size, m_size), glm::vec2(0.25, 1.0), "images/bat.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
			m_sprite->setNumberAnimations(2);
			m_sprite->setAnimationSpeed(FLY, 8);
			m_sprite->addKeyframe(FLY, glm::vec2(0.f, 0.f));
			m_sprite->addKeyframe(FLY, glm::vec2(0.25f, 0.f));
			m_sprite->addKeyframe(FLY, glm::vec2(0.5f, 0.f));
			m_sprite->addKeyframe(FLY, glm::vec2(0.75f, 0.f));
			m_sprite->setAnimationSpeed(RESTING, 8);
			m_sprite->addKeyframe(RESTING, glm::vec2(0.f, 0.f));
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

		void Watcher::FollowPlayer()
		{
			
			if (m_state == WatcherState::Quiet)
			{
				m_state = WatcherState::FollowingPlayer;
				m_sprite->changeAnimation(FLY);
				glm::ivec2 pos = m_player.GetPosition();
				m_dirWatcher = glm::ivec2(pos.x - m_posWatcher.x, pos.y - m_posWatcher.y);
				float tot_square_rot = std::sqrt(m_dirWatcher.x*m_dirWatcher.x/2 + m_dirWatcher.y*m_dirWatcher.y/2);
				m_dirWatcher.x /= tot_square_rot;
				m_dirWatcher.y /= tot_square_rot;
				m_destPos = pos;
			}
			else if (m_state == WatcherState::FollowingPlayer)
			{
				m_posWatcher = glm::vec2(m_posWatcher.x + m_dirWatcher.x, m_posWatcher.y + m_dirWatcher.y);
				m_sprite->setPosition(glm::vec2(float(m_tileMapDispl.x + m_posWatcher.x), float(m_tileMapDispl.y + m_posWatcher.y)));
				if (signbit(m_destPos.x - m_posWatcher.x) != signbit(m_dirWatcher.x) && signbit(m_destPos.y - m_posWatcher.y)  != signbit(m_dirWatcher.y))
				{
					m_state = WatcherState::LocatingPlayer;
				}
			}
			else if (m_state == WatcherState::LocatingPlayer)
			{
				glm::ivec2 pos = m_player.GetPosition();
				m_dirWatcher = glm::ivec2(pos.x - m_posWatcher.x, pos.y - m_posWatcher.y);
				float tot_square_rot = std::sqrt(m_dirWatcher.x*m_dirWatcher.x + m_dirWatcher.y*m_dirWatcher.y);
				m_dirWatcher.x /= tot_square_rot;
				m_dirWatcher.y /= tot_square_rot;
				m_destPos = pos;
				m_state = WatcherState::FollowingPlayer;
			}
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