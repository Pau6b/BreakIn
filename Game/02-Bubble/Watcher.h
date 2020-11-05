#pragma once
#include <memory>
#include "Sprite.h"
#include <math.h>
#include "CollisionManager.h"

namespace game
{
	namespace gameplay
	{
		class Player;
	}
}




namespace game
{
	namespace gameplay
	{
		class Watcher
		{
		public:
			enum ResultMovement
			{
				PlayerHit,
				None
			};

			Watcher(const Player& i_player, const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram, std::function<void()> i_LoseHP);
			void Update(int i_deltaTime);
			void Render();
			ResultMovement FollowPlayer();
			void SetPosition(const glm::vec2& i_pos);
			void Reset();


		private:

			enum class WatcherState
			{
				FollowingPlayer,
				LocatingPlayer,
				Quiet
			} m_state;

			ResultMovement m_player_state;

			glm::vec2 m_tileMapDispl, m_posWatcher;
			glm::vec2  m_dirWatcher, m_destPos;
			std::unique_ptr<visuals::Sprite> m_sprite;
			const Player& m_player;
			int m_size;
			float m_speed;
			const uint32_t k_timeToStartMoving = 30000;
			uint32_t m_currentTimeElapsed = 0;
			std::function<void()> m_LoseHp;
		};
	}
}

