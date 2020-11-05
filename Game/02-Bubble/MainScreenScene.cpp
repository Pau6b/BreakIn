#include "MainScreenScene.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SoundSystem.h"

namespace game
{
namespace gui
{

	MainScreenScene::MainScreenScene(sound::SoundSystem& i_soundSystem)
		: m_soundSystem(i_soundSystem)
	{

	}

void MainScreenScene::init()
{
	m_shaderProgram = std::make_unique<visuals::ShaderProgram>();
	InitShaders(*m_shaderProgram, "shaders/menu.vert", "shaders/menu.frag");
	m_playButtonInfo = {200, 200, 200, 100};
	m_buttonText = std::make_unique<visuals::Sprite>(glm::vec2(m_playButtonInfo.xWidth, m_playButtonInfo.yWidth), glm::vec2(1, 1), "images/UI/play_text.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGB, *m_shaderProgram);
	m_buttonText->setPosition(glm::vec2(m_playButtonInfo.xPos, m_playButtonInfo.yPos));
	m_projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void MainScreenScene::update(int i_deltaTime)
{
	//13 is enter
	if (core::Game::instance().getKey(13))
	{
		m_currentResult = core::Scene::SceneResult::GoToLevel;
	}
}

void MainScreenScene::render()
{
	glm::mat4 modelview;

	m_shaderProgram->use();
	m_shaderProgram->setUniformMatrix4f("projection", m_projection);
	m_shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	m_shaderProgram->setUniformMatrix4f("modelview", modelview);
	m_shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
	m_buttonText->render();
}

std::pair<core::Scene::SceneResult, uint32_t> MainScreenScene::GetSceneResult()
{
	return { m_currentResult, 1 };
}

void MainScreenScene::OnMouseButtonReleased(int32_t i_button)
{
	glm::ivec2 mousePos = core::Game::instance().getMousePos();
	if (IsMouseIsInButton(m_playButtonInfo, mousePos))
	{
		m_soundSystem.PlayMenuSounds(sound::MenuSounds::ButtonPressed);
		m_currentResult = core::Scene::SceneResult::GoToLevel;
	}
}

inline bool MainScreenScene::IsMouseIsInButton(const ButtonInfo& i_buttonInfo, const glm::ivec2& i_mousePosition)
{
	return (i_mousePosition.x > i_buttonInfo.xPos)
		&& (i_mousePosition.x < i_buttonInfo.xPos + i_buttonInfo.xWidth)
		&& (i_mousePosition.y > i_buttonInfo.yPos)
		&& (i_mousePosition.y < i_buttonInfo.yPos + i_buttonInfo.yWidth);
}

}
}
