#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "CheatSystem.h"
#include "SoundSystem.h"
#include "SoundHelpers.h"

namespace game
{
namespace core
{
void Game::init()
{
	m_bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	m_cheatSystem = std::make_unique<CheatSystem>();
	m_soundSystem = std::make_unique<sound::SoundSystem>();
	m_sceneManager = std::make_unique<SceneManager>("scenes/SceneConfig.txt", *m_cheatSystem, *m_soundSystem);
}

bool Game::update(int i_deltaTime)
{
	m_cheatSystem->Update(i_deltaTime);
	if (m_sceneManager)
	{
		m_sceneManager->Update(i_deltaTime);
	}

	
	return m_bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (m_sceneManager)
	{
		m_sceneManager->Render();
	}
}

void Game::keyPressed(int i_key)
{
	if(i_key == 27) // Escape code
		m_bPlay = false;
	m_keys[i_key] = true;
}

void Game::keyReleased(int i_key)
{
	m_keys[i_key] = false;

}

void Game::specialKeyPressed(int i_key)
{
	m_specialKeys[i_key] = true;
}

void Game::specialKeyReleased(int i_key)
{
	m_specialKeys[i_key] = false;
}

void Game::mouseMove(int i_x, int i_y)
{
	m_mousePos = glm::ivec2(i_x, i_y);
}

void Game::mousePress(int i_button, int i_x, int i_y)
{
	m_mousePos = glm::ivec2(i_x, i_y);
}

void Game::mouseRelease(int i_button, int i_x, int i_y)
{
	m_mousePos = glm::ivec2(i_x, i_y);
	m_sceneManager->OnMouseButtonReleased(i_button, m_mousePos);
}

bool Game::getKey(int i_key) const
{
	return m_keys[i_key];
}

bool Game::getSpecialKey(int i_key) const
{
	return m_specialKeys[i_key];
}

glm::ivec2 Game::getMousePos()
{
	return m_mousePos;
}

}
}



