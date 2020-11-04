#pragma once
#include <string>
#include <vector>
#include <map>
#include "LevelScene.h"
#include "Sounds.h"

namespace game
{
namespace core
{
class CheatSystem;
}
}

namespace game
{
namespace sound
{
class SoundSystem;
}
}


namespace game
{
namespace core
{
class SceneManager
{
public:
	SceneManager(const std::string& i_sceneConfigFilePath, const CheatSystem& i_cheatSystem, sound::SoundSystem& i_soundSystem);

	void Update(int i_deltaTime);
	void Render();
	void OnMouseButtonReleased(int32_t i_button, glm::ivec2 i_mousePosition);

private:
	std::unique_ptr<Scene> m_currentScene = nullptr;
	void ParseSceneConfigFilePath(const std::string& i_sceneConfigFilePath);
	const CheatSystem& m_cheatSystem;
	sound::SoundSystem& m_soundSystem;

	struct SceneConfig
	{
		SceneConfig(sound::BackgroundMusic i_backgroundSound);
		sound::BackgroundMusic backgroundSound = sound::BackgroundMusic::MainMenu;
	};

	struct LevelConfig : public SceneConfig
	{
		LevelConfig(const std::string& i_visualTilemapPath, const std::string& i_physicsTilemap, sound::BackgroundMusic i_backgroundMusic);
		std::string visualTilemapPath;
		std::string physicsMapPath;
	};

	struct GlobalScenesConfig
	{
		std::map<int32_t,LevelConfig> levels;
		std::unique_ptr<SceneConfig> mainMenu;
	} m_config;
};

}
}