#include "Door.h"
#include "Texture.h"
#include "Sprite.h"
#include <algorithm>
#include "SoundSystem.h"

namespace game
{
namespace visuals
{

Door::Door(uint32_t i_nblocks, std::string i_texturePath, PixelFormat i_pixelFormat, ShaderProgram& i_shaderProgram, sound::SoundSystem& i_soundSystem)
	: m_soundSystem(i_soundSystem)
{
	for (uint32_t i = 0; i < i_nblocks; ++i)
	{
		m_sprites.emplace_back(glm::vec2(16, 16), glm::vec2(1, 1), i_texturePath, i_pixelFormat, i_shaderProgram);
	}
}

void Door::Update(uint32_t i_elapsedTime)
{
	m_elapsedTime += i_elapsedTime;
	if (m_elapsedTime > k_timeToEraseBlock)
	{
		m_elapsedTime = 0;
		m_sprites.pop_back();
		m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BlockBroken);
	}
}

void Door::Render()
{
	std::for_each(std::begin(m_sprites), std::end(m_sprites), [](Sprite& i_sprite) { i_sprite.render(); });
}

void Door::SetPosition(glm::vec2 i_pos)
{
	for(uint32_t i = 0; i < m_sprites.size(); ++i)
	{
		const uint32_t offset = (m_sprites.size() - 1 - i)*16;
		m_sprites[i].setPosition(glm::vec2(i_pos.x+offset, i_pos.y));
	}
}

bool Door::HasAnimationFinished()
{
	return m_sprites.size() == 0;
}

}
}