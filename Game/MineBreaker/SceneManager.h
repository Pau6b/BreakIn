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
	SceneManager(const std::string& i_sceneConfigFilePath, CheatSystem& i_cheatSystem, sound::SoundSystem& i_soundSystem);

	void Update(int i_deltaTime);
	void Render();
	void OnMouseButtonReleased(int32_t i_button, glm::ivec2 i_mousePosition);

private:
	std::unique_ptr<Scene> m_currentScene = nullptr;
	void ParseSceneConfigFilePath(const std::string& i_sceneConfigFilePath);
	CheatSystem& m_cheatSystem;
	sound::SoundSystem& m_soundSystem;

	std::map<std::string, uint32_t> m_passwords;

	struct SceneConfig
	{
		SceneConfig(sound::BackgroundMusic i_backgroundSound);
		sound::BackgroundMusic backgroundSound = sound::BackgroundMusic::MainMenu;
	};

	struct LevelConfig : public SceneConfig
	{
		LevelConfig(const std::string& i_visualTilemapPath, const std::string& i_physicsTilemap, sound::BackgroundMusic i_backgroundMusic, const std::string& i_password);
		std::string visualTilemapPath;
		std::string physicsMapPath;
		std::string password;
	};

	struct GlobalScenesConfig
	{
		std::map<int32_t,LevelConfig> levels;
		std::unique_ptr<SceneConfig> mainMenu;
		std::unique_ptr<SceneConfig> controlsMenu;
		std::unique_ptr<SceneConfig> creditsMenu;
		std::unique_ptr<SceneConfig> passwordsMenu;
	} m_config;
};

}
}