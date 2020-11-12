#pragma once
#include "Entity.h"
#include <stdint.h>
#include <memory>
#include <glm/glm.hpp>
#include <functional>

namespace game
{
	namespace visuals
	{
		class Sprite;
		class ShaderProgram;
	}
	namespace gameplay
	{
		class Player;
		class Watcher;
	}
}

namespace game
{
	namespace sound
	{
		class SoundSystem;
	}
}

namespace game
{
	namespace gameplay
	{
		class Sensor : public Entity
		{
		public:
			Sensor(std::unique_ptr<visuals::Sprite> i_sprite, const glm::ivec2& i_tileMapDisplay, sound::SoundSystem& i_soundSystem, uint32_t i_currentMine);
			void Update(int i_deltaTime) override;
			void Render() override;
			void SetPosition(glm::vec2 i_position);
			void ActivateAlarm();
			void DesactivateAlarm();
			void InitWatcher(const Player& i_player, const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram, std::function<void()> i_loseHP);
		private:
			enum SensorState
			{
				ON,
				OFF
			} m_state;

			glm::vec2 m_position;
			const glm::ivec2 m_tileMapDisplay;
			std::unique_ptr<Watcher> m_watcher;
			std::unique_ptr<visuals::Sprite> m_sprite;
			sound::SoundSystem& m_soundSystem;
			uint32_t m_currentMine;
		};
	}
}