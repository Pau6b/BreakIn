#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

namespace game
{
namespace core
{
void Game::init()
{
	m_bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	m_sceneManager = std::make_unique<SceneManager>("scenes/SceneConfig.txt");
}

bool Game::update(int i_deltaTime)
{
	if (m_sceneManager)
	{
		m_sceneManager->update(i_deltaTime);
	}
	
	return m_bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (m_sceneManager)
	{
		m_sceneManager->render();
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
}

void Game::mousePress(int i_button)
{
}

void Game::mouseRelease(int i_button)
{
}

bool Game::getKey(int i_key) const
{
	return m_keys[i_key];
}

bool Game::getSpecialKey(int i_key) const
{
	return m_specialKeys[i_key];
}

}
}



