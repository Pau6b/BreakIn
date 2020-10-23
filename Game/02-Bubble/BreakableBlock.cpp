#include "BreakableBlock.h"
#include "Sprite.h"

namespace game
{
namespace gameplay
{

BreakableBlock::BreakableBlock(uint32_t i_resistance, std::unique_ptr<visuals::Sprite> i_sprite, const glm::ivec2& i_tileMapDisplay )
	: m_resistance(i_resistance)
	, m_sprite(std::move(i_sprite))
	, m_tileMapDisplay(i_tileMapDisplay)
{

}

void BreakableBlock::Update(int i_deltaTime)
{
}

void BreakableBlock::Render()
{
	m_sprite->render();
}

void BreakableBlock::SetResistance(uint32_t i_resistance)
{
	m_resistance = i_resistance;
}

uint32_t BreakableBlock::GetResistance() const
{
	return m_resistance;
}

void BreakableBlock::SetPosition(glm::vec2 i_position)
{
	m_position = i_position;
	m_sprite->setPosition(glm::vec2(float(m_tileMapDisplay.x + m_position.x), float(m_tileMapDisplay.y + m_position.y)));
}

}
}

