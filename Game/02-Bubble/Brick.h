#pragma once
#include <memory>
#include "Sprite.h"

namespace game
{
class Brick
{
public:
	Brick(ShaderProgram& i_shaderProgram, glm::ivec2 i_tileMapDisplay, uint32_t i_blockResistance);
	void SetPosition(glm::vec2 i_position);
	void SetResistance(uint32_t i_blockResistance);
	uint32_t GetResistance();
	void Render() const;
private:
	glm::vec2 m_position;
	glm::ivec2 m_tileMapDisplay;
	static const std::vector<std::string> k_texturePathsByHit;
	std::unique_ptr<Sprite> m_brickSprite;
	uint32_t m_blockResistance = 0;
};
}