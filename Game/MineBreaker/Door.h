#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"

namespace game
{
namespace visuals
{
class Sprite;
class ShaderProgram;
}
}

namespace game
{
namespace visuals
{
class Door
{
public:
	Door(uint32_t i_nblocks, std::string i_texturePath, PixelFormat i_pixelFormat, ShaderProgram& i_shaderProgram);
	void Update(uint32_t i_elapsedTime);
	void Render();
	void SetPosition(glm::vec2 i_pos);
private:
	std::vector<Sprite> m_sprites;
};
}
}