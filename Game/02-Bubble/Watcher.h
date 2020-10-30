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




namespace game
{
	namespace gameplay
	{
		class Watcher
		{
		public:
			Watcher(const Player& i_player);
			void Init(const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram);
			void Update(int i_deltaTime);
			void Render();

			void SetPosition(const glm::vec2& i_pos);
			void Reset();

		private:

			enum class WatcherState
			{
				FollowingPlayer,
				Quiet
			} m_state;

			glm::vec2 m_tileMapDispl, m_posWatcher;
			glm::vec2  m_dirWatcher;
			std::unique_ptr<visuals::Sprite> m_sprite;
			const Player& m_player;
			int m_size;
			float m_speed;
			const uint32_t k_timeToStartMoving = 30000;
			uint32_t m_currentTimeElapsed = 0;
		};
	}
}

