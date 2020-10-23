#pragma once
#include <memory>
#include <vector>
#include "ShaderProgram.h"
#include "BreakableBlock.h"

namespace game
{
namespace visuals
{
	class Sprite;
}
}

namespace game
{
namespace gameplay
{
class Brick : public BreakableBlock
{
public:
	Brick(visuals::ShaderProgram& i_shaderProgram, glm::ivec2 i_tileMapDisplay, uint32_t i_blockResistance);
private:
	static const std::vector<std::string> s_texturePathsByHit;
	std::unique_ptr<visuals::Sprite> m_brickSprite;
	uint32_t m_blockResistance = 0;
};
}
}