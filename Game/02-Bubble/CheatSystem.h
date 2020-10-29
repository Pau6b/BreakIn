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
private:
	bool m_inGodMode = false;
	bool m_lastGodModeState = false;
};
}
}