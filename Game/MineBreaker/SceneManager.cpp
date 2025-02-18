#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Log.h"
#include "LevelScene.h"
#include "MainScreenMenu.h"
#include "CheatSystem.h"
#include "SoundSystem.h"
#include "SoundHelpers.h"
#include "ControlsMenu.h"
#include "CreditsMenu.h"
#include "PasswordsMenu.h"

namespace game
{
namespace core
{
SceneManager::SceneManager(const std::string& i_sceneConfigFilePath, CheatSystem& i_cheatSystem, sound::SoundSystem& i_soundSystem)
	: m_cheatSystem(i_cheatSystem)
	, m_soundSystem(i_soundSystem)
{
	ParseSceneConfigFilePath(i_sceneConfigFilePath);
}

void SceneManager::Update(int i_deltaTime)
{
	if (m_currentScene)
	{

		m_currentScene->update(i_deltaTime);
		std::pair<Scene::SceneResult, uint32_t> sceneResult = m_cheatSystem.CheckLevelCheat();
		bool isCheat = true;
		if (sceneResult.first == Scene::SceneResult::NotFinished)
		{
			isCheat = false;
			sceneResult = m_currentScene->GetSceneResult();
		}
		if (sceneResult.first == Scene::SceneResult::GoToLevel)
		{
			auto it = m_config.levels.find(sceneResult.second);
			if(it == m_config.levels.end())
			{
				sceneResult.first = Scene::SceneResult::GoToMainMenu;
			}
		}
		switch (sceneResult.first)
		{
		case Scene::SceneResult::NotFinished:
			break;
		case Scene::SceneResult::GoToLevel:
		{
			Scene* currentScene = m_currentScene.get();
			gameplay::LevelScene* currentLevelScene = dynamic_cast<gameplay::LevelScene*>(currentScene);
			if (currentLevelScene && !isCheat)
			{
				m_currentScene = std::make_unique<gameplay::LevelScene>(m_config.levels.at(sceneResult.second).visualTilemapPath,
																		m_config.levels.at(sceneResult.second).physicsMapPath,
																		m_cheatSystem,
																		m_soundSystem,
																		sceneResult.second,
																		m_config.levels.at(sceneResult.second).password,
																		currentLevelScene->GetLevelResult());
			}
			else
			{
				m_currentScene = std::make_unique<gameplay::LevelScene>(m_config.levels.at(sceneResult.second).visualTilemapPath,
																		m_config.levels.at(sceneResult.second).physicsMapPath,
																		m_cheatSystem,
																		m_soundSystem,
																		sceneResult.second,
																		m_config.levels.at(sceneResult.second).password);
			}
			m_currentScene->init();
			m_config.levels.at(sceneResult.second);
			m_soundSystem.PlayBackgroundMusic(m_config.levels.at(sceneResult.second).backgroundSound);
			break;
		}
		case Scene::SceneResult::GoToMainMenu:
			m_currentScene = std::make_unique<gui::MainScreenMenu>(m_soundSystem);
			m_currentScene->init();
			m_soundSystem.PlayBackgroundMusic(m_config.mainMenu->backgroundSound);
			break;
		case Scene::SceneResult::GoToControlsScene:
			m_currentScene = std::make_unique<gui::ControlsMenu>(m_soundSystem);
			m_currentScene->init();
			m_soundSystem.PlayBackgroundMusic(m_config.controlsMenu->backgroundSound);
			break;
		case Scene::SceneResult::GoToCreditsScene:
			m_currentScene = std::make_unique<gui::CreditsMenu>(m_soundSystem);
			m_currentScene->init();
			m_soundSystem.PlayBackgroundMusic(m_config.creditsMenu->backgroundSound);
			break;
		case Scene::SceneResult::GoToPasswordsMenu:
			m_currentScene = std::make_unique<gui::PasswordsMenu>(m_soundSystem, m_passwords);
			m_currentScene->init();
			m_soundSystem.PlayBackgroundMusic(m_config.passwordsMenu->backgroundSound);
			break;
		}
	}
}

void SceneManager::Render()
{
	if (m_currentScene)
	{
		m_currentScene->render();
	}
}

void SceneManager::OnMouseButtonReleased(int32_t i_button, glm::ivec2 i_mousePosition)
{
	m_currentScene->OnMouseButtonReleased(i_button);
}

void SceneManager::ParseSceneConfigFilePath(const std::string& i_sceneConfigFilePath)
{
	std::ifstream file;
	file.open(i_sceneConfigFilePath);
	BreakIf(!file.is_open(), "No scene config file with path [" + i_sceneConfigFilePath + "] exists.\n");
	std::string line;
	while (std::getline(file, line))
	{
		if (line.rfind("MainMenu",0) == 0)
		{
			std::string backgroundSound;
			getline(file, backgroundSound);
			m_config.mainMenu = std::make_unique<SceneConfig>(sound::helpers::StringToBackgroundSound(backgroundSound));
		}
		else if (line.rfind("ControlsMenu", 0) == 0)
		{
			std::string backgroundSound;
			getline(file, backgroundSound);
			m_config.controlsMenu = std::make_unique<SceneConfig>(sound::helpers::StringToBackgroundSound(backgroundSound));
		}
		else if (line.rfind("CreditsMenu", 0) == 0)
		{
			std::string backgroundSound;
			getline(file, backgroundSound);
			m_config.creditsMenu = std::make_unique<SceneConfig>(sound::helpers::StringToBackgroundSound(backgroundSound));
		}
		else if (line.rfind("PasswordsMenu", 0) == 0)
		{
			std::string backgroundSound;
			getline(file, backgroundSound);
			m_config.passwordsMenu = std::make_unique<SceneConfig>(sound::helpers::StringToBackgroundSound(backgroundSound));
		}
		else if (line.rfind("Level",0) == 0)
		{
			std::istringstream sstream(line);
			std::string levelText;
			std::getline(sstream, levelText, ' ');
			std::getline(sstream, levelText);
			uint32_t levelNumber = std::stoi(levelText);
			std::string tilemapPath;
			std::string physicsPath;
			std::string backgroundSound;
			std::string password;
			std::getline(file, tilemapPath);
			std::getline(file, physicsPath);
			std::getline(file, backgroundSound);
			std::getline(file, password);
			m_passwords.emplace(password, levelNumber);
			m_config.levels.emplace(levelNumber, LevelConfig(tilemapPath, physicsPath, sound::helpers::StringToBackgroundSound(backgroundSound), password));
		}
		else
		{
			BreakIf(true, "Scene configuration file is not configured correctly, maybe an endl extra is missing?");
		}
		std::string blankLine;
		getline(file, blankLine);
	}

	m_currentScene = std::make_unique<gui::MainScreenMenu>(m_soundSystem);
	m_currentScene->init();
	m_soundSystem.PlayBackgroundMusic(m_config.mainMenu->backgroundSound);
	m_soundSystem.PlayBackgroundMusic(m_config.mainMenu->backgroundSound);
	
}

SceneManager::LevelConfig::LevelConfig(const std::string& i_visualTilemapPath, const std::string& i_physicsTilemap, sound::BackgroundMusic i_backgroundMusic, const std::string& i_password)
	: SceneConfig(i_backgroundMusic)
	, visualTilemapPath(i_visualTilemapPath)
	, physicsMapPath(i_physicsTilemap)
	, password(i_password)
{
}

SceneManager::SceneConfig::SceneConfig(sound::BackgroundMusic i_backgroundSound)
	: backgroundSound(i_backgroundSound)
{

}

}
}