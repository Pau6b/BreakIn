#include "Portal.h"
#include "Sprite.h"

namespace game
{
	namespace gameplay
	{

		const std::vector<std::string> Portal::m_spritePaths = { "images/PortalAnimation.png", "images/PortalAnimation.png", "images/PortalAnimation.png" };

		Portal::Portal(uint32_t m_currentMine, glm::vec2 i_referenceVector, visuals::ShaderProgram& i_shaderProgram, const glm::ivec2& i_tileMapDisplay)
			: m_referenceVector(i_referenceVector)
			, m_tileMapDisplay(i_tileMapDisplay)
		{
			m_sprite = std::make_unique<visuals::Sprite>(glm::vec2(16, 16), glm::vec2(1.f / 6, 1.f / 5), m_spritePaths[m_currentMine - 1], visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
			m_sprite->setNumberAnimations(1);
			m_sprite->setAnimationSpeed(0, 20);
			m_sprite->addKeyframe(0, glm::vec2(0,0));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6,0));
			m_sprite->addKeyframe(0, glm::vec2(2.f / 6,0));
			m_sprite->addKeyframe(0, glm::vec2(3.f / 6,0));
			m_sprite->addKeyframe(0, glm::vec2(4.f / 6,0));
			m_sprite->addKeyframe(0, glm::vec2(5.f / 6,0));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6, 1.f/5));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6, 1.f/5));
			m_sprite->addKeyframe(0, glm::vec2(2.f / 6, 1.f/5));
			m_sprite->addKeyframe(0, glm::vec2(3.f / 6, 1.f/5));
			m_sprite->addKeyframe(0, glm::vec2(4.f / 6, 1.f/5));
			m_sprite->addKeyframe(0, glm::vec2(5.f / 6, 1.f/5));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6, 2.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6, 2.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(2.f / 6, 2.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(3.f / 6, 2.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(4.f / 6, 2.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(5.f / 6, 2.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6, 3.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6, 3.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(2.f / 6, 3.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(3.f / 6, 3.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(4.f / 6, 3.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(5.f / 6, 3.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6, 4.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(1.f / 6, 4.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(2.f / 6, 4.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(3.f / 6, 4.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(4.f / 6, 4.f / 5));
			m_sprite->addKeyframe(0, glm::vec2(5.f / 6, 4.f / 5));
			m_sprite->changeAnimation(0);
		}

		void Portal::Update(int i_deltaTime)
		{
			m_sprite->update(i_deltaTime);
		}

		void Portal::Render()
		{
			m_sprite->render();
		}


		const glm::vec2 Portal::GetReferenceVector() const
		{
			return m_referenceVector;
		}

		void Portal::SetPosition(const glm::vec2& i_pos)
		{
			m_pos = i_pos;
			m_sprite->setPosition(glm::vec2(float(m_tileMapDisplay.x + m_pos.x), float(m_tileMapDisplay.y + m_pos.y)));
		}

		const glm::vec2 Portal::GetPos() const
		{
			return m_pos;
		}

	}

}

