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
namespace sound
{
class SoundSystem;
}
}

namespace game
{
namespace visuals
{
class Door
{
public:
	Door(uint32_t i_nblocks, std::string i_texturePath, PixelFormat i_pixelFormat, ShaderProgram& i_shaderProgram, sound::SoundSystem& i_soundSystem);
	void Update(uint32_t i_elapsedTime);
	void Render();
	void SetPosition(glm::vec2 i_pos);
	bool HasAnimationFinished();
private:
	std::vector<Sprite> m_sprites;
	const uint32_t k_timeToEraseBlock = 150;
	int32_t m_elapsedTime;
	sound::SoundSystem& m_soundSystem;
};
}
}