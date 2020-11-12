#pragma once
#include "Scene.h"
#include <array>

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
	std::pair<Scene::SceneResult, uint32_t> CheckLevelCheat();
private:
	bool m_inGodMode = false;
	bool m_lastGodModeState = false;
	bool m_lastUp = false;
	bool m_lastDown = false;
	std::array<bool, 3> m_levelResults = {false,false,false};
};
}
}