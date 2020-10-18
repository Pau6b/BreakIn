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
	Brick(visuals::ShaderProgram& i_shaderProgram, glm::ivec2 i_tileMapDisplay, uint32_t i_blockResistance);
	void SetPosition(glm::vec2 i_position);
	void SetResistance(uint32_t i_blockResistance);
	uint32_t GetResistance();
	void Render() const;
private:
	glm::vec2 m_position;
	glm::ivec2 m_tileMapDisplay;
	static const std::vector<std::string> k_texturePathsByHit;
	std::unique_ptr<visuals::Sprite> m_brickSprite;
	uint32_t m_blockResistance = 0;
};
}
}