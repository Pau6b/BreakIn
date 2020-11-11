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

bool CheatSystem::InGodMode() const
{
	return m_inGodMode;
}

}
}