#include "Brick.h"
#include "Sprite.h"

namespace game
{
namespace gameplay
{
	const std::vector<std::string> Brick::s_texturePathsByHit = { "images/WoodBreakable.png", "images/cobblestoneBreakable.png", "images/bricksBreakable.png" };
	
	Brick::Brick(visuals::ShaderProgram& i_shaderProgram, glm::ivec2 i_tileMapDisplay, uint32_t i_blockResistance)
		: BreakableBlock(i_blockResistance,std::make_unique<visuals::Sprite>(glm::vec2(32, 16), glm::vec2(1, 1), s_texturePathsByHit[i_blockResistance - 1], visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGB, i_shaderProgram), BreakableBlock::BreakableBlockAnimationSize::Small, i_tileMapDisplay, i_shaderProgram)
	{
	}


}
}