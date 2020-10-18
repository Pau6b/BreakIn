#pragma once
#include <memory>
#include "Sprite.h"

namespace game
{
namespace gameplay
{
class Brick
{
public:
	Brick(uint32_t i_blockResistance, visuals::ShaderProgram& i_shaderProgram);

private:
	static const std::vector<std::string> k_texturePathsByHit;
	std::unique_ptr<visuals::Sprite> m_brickSprite;
	uint32_t m_blockResistance;
};
}
}