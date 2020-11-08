#include "Button.h"

namespace game
{
namespace gui
{

Button::Button(int32_t i_xPos, int32_t i_yPos, int32_t i_xWidth, int32_t i_yWidth, visuals::ShaderProgram& i_shaderProgram)
	: m_info{i_xPos,i_yPos,i_xWidth,i_yWidth}
	, m_buttonSprite(glm::vec2(i_xWidth, i_yWidth),glm::vec2(1,1),"images/UI/Button.png",visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram)
	, m_hoveredSprite(glm::vec2(i_xWidth, i_yWidth),glm::vec2(1,1),"images/UI/HoveredButton.png",visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, i_shaderProgram)
{
	m_buttonSprite.setPosition(glm::vec2(i_xPos, i_yPos));
	m_hoveredSprite.setPosition(glm::vec2(i_xPos, i_yPos));
}

void Button::UpdateMousePosition(const glm::ivec2& i_mousePosition)
{
	if (IsMouseIsInButton(m_info, i_mousePosition))
	{
		if (m_buttonState == ButtonState::Unpressed)
		{
			m_buttonState = ButtonState::Hovered;
		}
	}
	else 
	{
		if (m_buttonState == ButtonState::Hovered)
		{
			m_buttonState = ButtonState::Unpressed;
		}
	}
}

bool Button::IsMousePressed(const glm::ivec2& i_mousePosition) const
{
	return IsMouseIsInButton(m_info, i_mousePosition);
}

void Button::Render()
{
	m_buttonSprite.render();
	if (m_buttonState == ButtonState::Hovered)
	{
		m_hoveredSprite.render();
	}

}

void Button::SetPlainText(std::string i_text)
{
	m_buttonText = i_text;
}

void Button::SetTextMargin(glm::vec2 i_textMargin)
{
	m_textMargin = i_textMargin;
}

bool Button::IsMouseIsInButton(const ButtonInfo& i_buttonInfo, const glm::ivec2& i_mousePosition)
{
	return (i_mousePosition.x > i_buttonInfo.xPos)
		&& (i_mousePosition.x < i_buttonInfo.xPos + i_buttonInfo.xWidth)
		&& (i_mousePosition.y > i_buttonInfo.yPos)
		&& (i_mousePosition.y < i_buttonInfo.yPos + i_buttonInfo.yWidth);
}


}
}