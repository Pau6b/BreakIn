#include "Brick.h"

namespace game
{

	Brick::Brick(uint32_t i_blockResistance, ShaderProgram& i_shaderProgram)
		: m_blockResistance(i_blockResistance)
	{
		m_brickSprite = std::make_unique<Sprite>(glm::vec2(32, 16), glm::vec2(1, 1), k_texturePathsByHit[i_blockResistance-1], PixelFormat::TEXTURE_PIXEL_FORMAT_RGB, i_shaderProgram );
	}

	const std::vector<std::string> Brick::k_texturePathsByHit = { "images/WoodBreakable.png", "images/cobblestoneBreakable.png", "images/bricksBreakable.png" };

}