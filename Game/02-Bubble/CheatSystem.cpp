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

bool CheatSystem::InGodMode() const
{
	return m_inGodMode;
}

}
}