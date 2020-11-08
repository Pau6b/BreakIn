#include "CheatSystem.h"
#include "Game.h"

namespace game
{
namespace core
{

void CheatSystem::Update(int i_deltaTime)
{
	bool goodModeKeyPressed = Game::instance().getKey('g');
	if (goodModeKeyPressed != m_lastGodModeState)
	{
		if (goodModeKeyPressed)
		{
			m_inGodMode = !m_inGodMode;
		}
		m_lastGodModeState = goodModeKeyPressed;
	}

}

bool CheatSystem::CheckUp()
{
	bool MoveUpKeyPressed = Game::instance().getKey('w');
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
	bool MoveDownKeyPressed = Game::instance().getKey('s');
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