#pragma once
#include <string>
#include <vector>
#include <map>
#include "LevelScene.h"

namespace game
{
namespace core
{
	class CheatSystem;
}
}

namespace game
{
namespace core
{
class SceneManager
{
public:
	SceneManager(const std::string& i_sceneConfigFilePath, const CheatSystem& i_cheatSystem);

	void Update(int i_deltaTime);
	void Render();
	void OnMouseButtonReleased(int32_t i_button);

private:
	std::unique_ptr<Scene> m_currentScene = nullptr;
	void ParseSceneConfigFilePath(const std::string& i_sceneConfigFilePath);
	const CheatSystem& m_cheatSystem;

	struct LevelConfig
	{
		LevelConfig(const std::string& i_visualTilemapPath, const std::string& i_physicsTilemap);
		std::string visualTilemapPath;
		std::string physicsMapPath;
	};

	struct SceneConfig
	{
		std::map<int32_t,LevelConfig> levels;
	} m_config;
};

}
}