#pragma once
#include <stdint.h>
#include <glm/glm.hpp>
#include "Sprite.h"
#include "Text.h"

namespace game
{
namespace gui
{

class Button
{
public:
	Button(int32_t i_xPos, int32_t i_yPos, int32_t i_xWidth, int32_t i_yWidth, visuals::ShaderProgram& i_shaderProgram);


	void UpdateMousePosition(const glm::ivec2& i_mousePosition);
	bool IsMousePressed(const glm::ivec2& i_mousePosition) const;
	void Render();
	void SetPlainText(std::string i_text);
	void SetTextMargin(glm::vec2 i_textMargin);

private:
	
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
	ButtonState m_buttonState = ButtonState::Unpressed;
	std::string m_buttonText;
	glm::vec2 m_textMargin = glm::vec2(0,0);

	static inline bool IsMouseIsInButton(const ButtonInfo& i_buttonInfo, const glm::ivec2& i_mousePosition);
	
};

}
}