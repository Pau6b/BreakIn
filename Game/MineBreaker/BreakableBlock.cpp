#include "BreakableBlock.h"
#include "Sprite.h"
#include <functional>
#include <algorithm>

namespace game
{
namespace gameplay
{

BreakableBlock::BreakableBlock(uint32_t i_resistance, std::unique_ptr<visuals::Sprite> i_sprite,BreakableBlockAnimationSize i_animationSize, const glm::ivec2& i_tileMapDisplay, visuals::ShaderProgram& i_shaderProgram )
	: m_resistance(i_resistance)
	, m_animationResistance(i_resistance)
	, m_sprite(std::move(i_sprite))
	, m_tileMapDisplay(i_tileMapDisplay)
{
	switch (i_animationSize)
	{
	case BreakableBlockAnimationSize::Big:
		m_breakAnimationSprite = std::make_unique<visuals::Sprite>(glm::vec2(32, 32), glm::vec2(0.5, 0.5), "images/BreakAnimationBig.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
		m_breakAnimationSprite->setNumberAnimations(1);
		m_breakAnimationSprite->setAnimationSpeed(0, 20);
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0, 0));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0.5f, 0));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0, 0.5f));
		m_breakAnimationSprite->changeAnimation(0);
		break;
	case BreakableBlockAnimationSize::Small:
		m_breakAnimationSprite = std::make_unique<visuals::Sprite>(glm::vec2(32, 16), glm::vec2(0.5, 0.2), "images/BreakAnimationSmall.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram);
		m_breakAnimationSprite->setNumberAnimations(1);
		m_breakAnimationSprite->setAnimationSpeed(0, 20);
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0, 0));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0.5f, 0));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0, 0.2f));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0.5f, 0.2f));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0, 0.4f));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0.5f, 0.4f));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0, 0.6f));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0.5f, 0.6f));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0, 0.8f));
		m_breakAnimationSprite->addKeyframe(0, glm::vec2(0.5f, 0.8f));
		m_breakAnimationSprite->changeAnimation(0);
		m_breakAnimationSprite->setCurrentKeyFrame(std::max((3-int32_t(m_resistance))*3-1,0));
		break;
	case BreakableBlockAnimationSize::None:
		break;
	}
	if (m_breakAnimationSprite)
	{
		m_breakAnimationSprite->SetAnimationAdvanceType(visuals::Sprite::AnimationAdvanceType::NumberOfFrames);
		m_breakAnimationSprite->SetOnFramesAdvancedCallback(std::bind(&BreakableBlock::OnFramesAdvanced, this, std::placeholders::_1));
	}
}

BreakableBlock::BreakableBlock(uint32_t i_resistance, std::unique_ptr<visuals::Sprite> i_sprite, const glm::ivec2& i_tileMapDisplay)
	: m_resistance(i_resistance)
	, m_animationResistance(0)
	, m_sprite(std::move(i_sprite))
	, m_tileMapDisplay(i_tileMapDisplay)
{

}

void BreakableBlock::Update(int i_deltaTime)
{
	if (m_breakAnimationSprite && m_animationResistance != m_resistance)
	{
		m_breakAnimationSprite->update(i_deltaTime);
	}
}

void BreakableBlock::Render()
{
	m_sprite->render();
	if (m_breakAnimationSprite && m_isDamaged)
	{
		m_breakAnimationSprite->render();
	}
	
}

void BreakableBlock::SetResistance(int32_t i_resistance)
{
	if (m_breakAnimationSprite && m_resistance > 0)
	{
		if (!m_isDamaged)
		{
			m_breakAnimationSprite->SetFramesToAdvance(2);
			m_isDamaged = true;
			m_resistance--;
		}
		if (m_resistance != i_resistance)
		{
			int32_t resistanceCapped = std::max(i_resistance, 0);
			uint32_t leftResistanceToAdvance = m_resistance - resistanceCapped;
			m_resistance = resistanceCapped;
			m_breakAnimationSprite->SetFramesToAdvance(3*leftResistanceToAdvance);
		}
	}
	m_resistance = i_resistance;
}

int32_t BreakableBlock::GetResistance() const
{
	if (m_breakAnimationSprite)
	{
		return m_animationResistance;
	}
	else
	{
		return m_resistance;
	}
}

void BreakableBlock::SetPosition(glm::vec2 i_position)
{
	m_position = i_position;
	m_sprite->setPosition(glm::vec2(float(m_tileMapDisplay.x + m_position.x), float(m_tileMapDisplay.y + m_position.y)));
	if (m_breakAnimationSprite)
	{
		m_breakAnimationSprite->setPosition(glm::vec2(float(m_tileMapDisplay.x + m_position.x), float(m_tileMapDisplay.y + m_position.y)));
	}
}

void BreakableBlock::OnFramesAdvanced(uint32_t i_numberOfFramesAdvanced)
{
	if (m_firstBreak)
	{
		i_numberOfFramesAdvanced++;
	}
	uint32_t resistanceAdvanced = i_numberOfFramesAdvanced / 3;
	m_animationResistance -= resistanceAdvanced;
	m_firstBreak = false;
}

}
}

