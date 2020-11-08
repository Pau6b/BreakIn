#pragma once

namespace game
{
namespace core
{
class CheatSystem
{
public:
	void Update(int i_deltaTime);
	bool InGodMode() const;
	bool CheckUp();
	bool CheckDown();
private:
	bool m_inGodMode = false;
	bool m_lastGodModeState = false;
	bool m_lastUp = false;
	bool m_lastDown = false;
};
}
}