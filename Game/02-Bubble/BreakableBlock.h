#pragma once
#include "Entity.h"
#include <stdint.h>
#include <memory>
#include <glm/glm.hpp>

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
	class BreakableBlock : public Entity
	{
	public:
		BreakableBlock(uint32_t i_resistance, std::unique_ptr<visuals::Sprite> i_sprite, const glm::ivec2& i_tileMapDisplay);
		void Update(int i_deltaTime) override;
		void Render() override;
		void SetResistance(uint32_t i_resistance);
		uint32_t GetResistance() const;
		void SetPosition(glm::vec2 i_position);
	private:
		glm::vec2 m_position;
		const glm::ivec2 m_tileMapDisplay;
		uint32_t m_resistance;
		std::unique_ptr<visuals::Sprite>  m_sprite;
	};
}
}