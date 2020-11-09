#pragma once
#include <stdint.h>
#include <glm/glm.hpp>
#include "Sprite.h"
#include "Text.h"
#include <map>

namespace game
{
namespace gui
{

class Button
{
public:
	enum class ButtonText
	{
		Play,
		Back,
		Credits,
		Passwords,
		Controls
	};
	Button(int32_t i_xPos, int32_t i_yPos, int32_t i_xWidth, int32_t i_yWidth, visuals::ShaderProgram& i_shaderProgram, ButtonText i_buttonText);


	void UpdateMousePosition(const glm::ivec2& i_mousePosition);
	bool IsMousePressed(const glm::ivec2& i_mousePosition) const;
	void Render();

private:
	
	static std::map<ButtonText, std::string> m_buttonTextPaths;
	enum class ButtonState
	{
		Unpressed,
		Hovered,
		Pressed
	};


	struct ButtonInfo
	{
		int32_t xPos;
		int32_t yPos;
		int32_t xWidth;
		int32_t yWidth;
	} m_info;

	visuals::Sprite m_buttonSprite;
	visuals::Sprite m_hoveredSprite;
	visuals::Sprite m_textSprite;
	ButtonState m_buttonState = ButtonState::Unpressed;
	glm::vec2 m_textMargin = glm::vec2(0,0);

	static inline bool IsMouseIsInButton(const ButtonInfo& i_buttonInfo, const glm::ivec2& i_mousePosition);
	
};

}
}