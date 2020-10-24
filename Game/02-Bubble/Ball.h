#pragma once
#include <memory>
#include "Sprite.h"
#include "CollisionManager.h"


// Ball is a sprite


namespace game
{
	namespace gameplay
	{
		class Ball
		{
		public:
			Ball(physics::CollisionManager& i_collisionsManager);
			void init(const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram);
			void update(int i_deltaTime);
			void render();

			void setPosition(const glm::vec2& i_pos);

		private:
			glm::ivec2 m_tileMapDispl, m_posBall;
			glm::vec2  m_dirBall;
			std::unique_ptr<visuals::Sprite> m_sprite;
			physics::CollisionManager& m_map;
			int m_size, m_speed;
		};
	}
}

