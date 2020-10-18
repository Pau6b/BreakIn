#include "Brick.h"

namespace game
{
namespace gameplay
{
	Brick::Brick(uint32_t i_blockResistance, visuals::ShaderProgram& i_shaderProgram)
		: m_blockResistance(i_blockResistance)
	{
		m_brickSprite = std::make_unique<visuals::Sprite>(glm::vec2(32, 16), glm::vec2(1, 1), k_texturePathsByHit[i_blockResistance - 1], visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGB, i_shaderProgram);
	}

	const std::vector<std::string> Brick::k_texturePathsByHit = { "images/WoodBreakable.png", "images/cobblestoneBreakable.png", "images/bricksBreakable.png" };

}
}