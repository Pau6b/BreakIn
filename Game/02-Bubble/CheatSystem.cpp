#include "CheatSystem.h"
#include "Game.h"

namespace game
{
namespace core
{

void CheatSystem::Update(int i_deltaTime)
{
	bool godModeKeyPressed = Game::instance().getKey('g') || Game::instance().getKey('G');
	if (godModeKeyPressed != m_lastGodModeState )
	{
		if (godModeKeyPressed)
		{
			m_inGodMode = !m_inGodMode;
		}
		m_lastGodModeState = godModeKeyPressed;
	}

}

bool CheatSystem::CheckUp()
{
	bool MoveUpKeyPressed = Game::instance().getKey('w') || Game::instance().getKey('W');
	if (m_lastUp != MoveUpKeyPressed)
	{
		m_lastUp = MoveUpKeyPressed;
		if (MoveUpKeyPressed)
		{
			return MoveUpKeyPressed;
		}
	}
	return false;
}


bool CheatSystem::CheckDown()
{
	bool MoveDownKeyPressed = Game::instance().getKey('s') || Game::instance().getKey('S');
	if (m_lastDown != MoveDownKeyPressed)
	{
		m_lastDown = MoveDownKeyPressed;
		if (MoveDownKeyPressed)
		{
			return MoveDownKeyPressed;
		}
	}
	return false;
}

std::pair<Scene::SceneResult, glm::uint32_t> CheatSystem::CheckLevelCheat()
{
	Game& gameInstance = Game::instance();
	bool isOnePressed = gameInstance.getKey('1');
	if (isOnePressed && !m_levelResults[0])
	{
		m_levelResults[0] = true;
		return{ Scene::SceneResult::GoToLevel, 1 };
	}
	else if (!isOnePressed)
	{
		m_levelResults[0] = false;
	}
	bool isTwoPressed = gameInstance.getKey('2');
	if (isTwoPressed && !m_levelResults[1])
	{
		m_levelResults[1] = true;
		return{ Scene::SceneResult::GoToLevel, 2 };
	}
	else if (!isTwoPressed)
	{
		m_levelResults[1] = false;
	}
	bool isThreePresed = gameInstance.getKey('3');
	if ( isThreePresed && !m_levelResults[2])
	{
		m_levelResults[2] = true;
		return{ Scene::SceneResult::GoToLevel, 3 };
	}
	else if (!isThreePresed)
	{
		m_levelResults[2] = false;
	}
	return { Scene::SceneResult::NotFinished, 0 };
}

bool CheatSystem::InGodMode() const
{
	return m_inGodMode;
}

}
}