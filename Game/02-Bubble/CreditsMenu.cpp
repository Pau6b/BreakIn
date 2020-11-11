#include "CreditsMenu.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SoundSystem.h"
#include "Button.h"
#include <algorithm>

namespace game
{
namespace gui
{

CreditsMenu::CreditsMenu(sound::SoundSystem& i_soundSystem)
	: m_soundSystem(i_soundSystem)
{

}

void CreditsMenu::init()
{
	m_shaderProgram = std::make_unique<visuals::ShaderProgram>();
	InitShaders(*m_shaderProgram, "shaders/menu.vert", "shaders/menu.frag");
	m_background = std::make_unique<visuals::Sprite>(glm::vec2(640, 480), glm::vec2(1, 1), "images/UI/CreditsBackground.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, *m_shaderProgram);
	m_credits = std::make_unique<visuals::Sprite>(glm::vec2(640, 480), glm::vec2(1, 1), "images/UI/Credits.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, *m_shaderProgram);
	std::unique_ptr<Button> controlsButton = std::make_unique<Button>(170, 380, 300, 33, *m_shaderProgram, Button::ButtonText::Back);
	m_buttons.emplace_back(std::move(controlsButton), core::Scene::SceneResult::GoToMainMenu);
	m_projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void CreditsMenu::update(int i_deltaTime)
{
	//13 is enter
	if (core::Game::instance().getKey(13))
	{
		m_currentResult = core::Scene::SceneResult::GoToLevel;
	}
}

void CreditsMenu::render()
{
	glm::mat4 modelview;
	UpdateButtonMousePositions();
	m_shaderProgram->use();
	m_shaderProgram->setUniformMatrix4f("projection", m_projection);
	m_shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	m_shaderProgram->setUniformMatrix4f("modelview", modelview);
	m_shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
	m_background->render();
	std::for_each(std::begin(m_buttons), std::end(m_buttons), [](ButtonAction& i_button) {
		i_button.button->Render();
	});
	m_credits->render();
}

std::pair<core::Scene::SceneResult, uint32_t> CreditsMenu::GetSceneResult()
{
	return{ m_currentResult, 1 };
}

void CreditsMenu::OnMouseButtonReleased(int32_t i_button)
{
	glm::ivec2 mousePos = core::Game::instance().getMousePos();
	for (const auto& button : m_buttons)
	{
		if (button.button->IsMousePressed(mousePos))
		{
			m_soundSystem.PlayMenuSounds(sound::MenuSounds::ButtonPressed);
			m_currentResult = button.sceneAction;
		}
	}

}

void CreditsMenu::UpdateButtonMousePositions()
{
	glm::vec2 mousePos = core::Game::instance().getMousePos();
	std::for_each(std::begin(m_buttons), std::end(m_buttons), [mousePos](ButtonAction& i_button) { i_button.button->UpdateMousePosition(mousePos); });
}

CreditsMenu::ButtonAction::ButtonAction(std::unique_ptr<Button> i_button, core::Scene::SceneResult i_sceneAction)
	: button(std::move(i_button))
	, sceneAction(i_sceneAction)
{

}

}
}
