#pragma once
#include "Entity.h"
#include <glm/glm.hpp>
#include <stdint.h>
#include <memory>
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
namespace gameplay
{
	class BreakableBlock : public Entity
	{
	public:
		enum class BreakableBlockAnimationSize
		{
			Big,
			Small,
			None
		};

		BreakableBlock(uint32_t i_resistance, std::unique_ptr<visuals::Sprite> i_sprite, BreakableBlockAnimationSize i_animationSize, const glm::ivec2& i_tileMapDisplay, visuals::ShaderProgram& i_shaderProgram);
		BreakableBlock(uint32_t i_resistance, std::unique_ptr<visuals::Sprite> i_sprite, const glm::ivec2& i_tileMapDisplay);
		void Update(int i_deltaTime) override;
		void Render() override;
		void SetResistance(int32_t i_resistance);
		int32_t GetResistance() const;
		void SetPosition(glm::vec2 i_position);
		void OnFramesAdvanced(uint32_t i_numberOfFramesAdvanced);
	private:
		glm::vec2 m_position;
		const glm::ivec2 m_tileMapDisplay;
		uint32_t m_resistance;
		bool m_isDamaged = false;
		std::unique_ptr<visuals::Sprite> m_sprite;
		std::unique_ptr<visuals::Sprite> m_breakAnimationSprite;
		bool m_isInAnimation;
		int32_t m_animationResistance;
		bool m_firstBreak = true;
	};
}
}