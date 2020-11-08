#include "Sensor.h"
#include "Sprite.h"
#include "Watcher.h"
#include "Sounds.h"
#include "SoundSystem.h"

#define WATCHER_INITIAL_POS_X 1
#define WATCHER_INITIAL_POS_Y 1

namespace game
{
	namespace gameplay
	{
		Sensor::Sensor(std::unique_ptr<visuals::Sprite> i_sprite, const glm::ivec2& i_tileMapDisplay, sound::SoundSystem& i_soundSystem)
			: m_sprite(std::move(i_sprite))
			, m_state(SensorState::OFF)
			, m_tileMapDisplay(i_tileMapDisplay)
			, m_soundSystem(i_soundSystem)
		{
			m_sprite->setNumberAnimations(2);
			m_sprite->setAnimationSpeed(ON, 8);
			m_sprite->addKeyframe(ON, glm::vec2(1 / 7.f, 0.f));
			m_sprite->addKeyframe(ON, glm::vec2(2 / 7.f, 0.f));
			m_sprite->addKeyframe(ON, glm::vec2(3 / 7.f, 0.f));
			m_sprite->addKeyframe(ON, glm::vec2(4 / 7.f, 0.f));
			m_sprite->addKeyframe(ON, glm::vec2(5 / 7.f, 0.f));
			m_sprite->addKeyframe(ON, glm::vec2(6 / 7.f, 0.f));
			m_sprite->setAnimationSpeed(OFF, 8);
			m_sprite->addKeyframe(OFF, glm::vec2(0.f, 0.f));
			m_sprite->changeAnimation(OFF);
		}

		void Sensor::Update(int i_deltaTime)
		{
			if (m_state == SensorState::ON)
			{
				Watcher::ResultMovement resultado =  m_watcher->FollowPlayer();
				if (resultado == Watcher::ResultMovement::PlayerHit) {
					DesactivateAlarm();
				}
			}
			m_sprite->update(i_deltaTime);
			m_watcher->Update(i_deltaTime);
		}

		void Sensor::Render()
		{
			m_sprite->render();
			m_watcher->Render();
		}

		void Sensor::SetPosition(glm::vec2 i_position)
		{
			m_position = i_position;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDisplay.x + m_position.x), float(m_tileMapDisplay.y + m_position.y)));
		}

		void Sensor::ActivateAlarm()
		{
			if (m_state == SensorState::OFF)
			{
				m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::AlarmAcivated);
				m_sprite->changeAnimation(ON);
				m_state = SensorState::ON;
				m_watcher->FollowPlayer();
			}
		}

		void Sensor::DesactivateAlarm()
		{
			m_sprite->changeAnimation(OFF);
			m_state = SensorState::OFF;
			m_watcher->Reset();
		}

		void Sensor::InitWatcher(const Player& i_player, const glm::ivec2& i_tileMapPos, visuals::ShaderProgram& i_shaderProgram, std::function<void()> LoseHp)
		{
			m_watcher = std::make_unique<Watcher>(i_player, i_tileMapPos, i_shaderProgram, LoseHp);
			m_watcher->SetPosition(glm::vec2(WATCHER_INITIAL_POS_X* 16, WATCHER_INITIAL_POS_Y * 16));
		}

	}
}

