#include "Brick.h"

namespace game
{
namespace gameplay
{
	Brick::Brick(visuals::ShaderProgram& i_shaderProgram, glm::ivec2 i_tileMapDisplay, uint32_t i_blockResistance)
		: m_tileMapDisplay(i_tileMapDisplay)
		, m_blockResistance(i_blockResistance)
	{
		m_brickSprite = std::make_unique<visuals::Sprite>(glm::vec2(32, 16), glm::vec2(1, 1), k_texturePathsByHit[i_blockResistance - 1], visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGB, i_shaderProgram);
	}

	const std::vector<std::string> Brick::k_texturePathsByHit = { "images/WoodBreakable.png", "images/cobblestoneBreakable.png", "images/bricksBreakable.png" };

	void Brick::SetPosition(glm::vec2 i_position)
	{
		m_position = i_position;
		m_brickSprite->setPosition(glm::vec2(float(m_tileMapDisplay.x + m_position.x), float(m_tileMapDisplay.y + m_position.y)));
	}

	void Brick::SetResistance(uint32_t i_blockResistance)
	{
		m_blockResistance = i_blockResistance;
	}

	uint32_t Brick::GetResistance()
	{
		return m_blockResistance;
	}

	void Brick::Render() const
	{
		m_brickSprite->render();
	}

}
}