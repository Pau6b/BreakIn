#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "LevelScene.h"
#include "MainScreenScene.h"

namespace game
{
namespace core
{
SceneManager::SceneManager(const std::string& i_sceneConfigFilePath)
{
	ParseSceneConfigFilePath(i_sceneConfigFilePath);
}

void SceneManager::Update(int i_deltaTime)
{
	if (m_currentScene)
	{
		m_currentScene->update(i_deltaTime);
		std::pair<Scene::SceneResult, uint32_t> sceneResult = m_currentScene->GetSceneResult();
		switch (sceneResult.first)
		{
		case Scene::SceneResult::NotFinished:
			break;
		case Scene::SceneResult::GoToLevel:
		{
			m_currentScene = std::make_unique<gameplay::LevelScene>(m_config.levels.at(sceneResult.second).visualTilemapPath, m_config.levels.at(sceneResult.second).physicsMapPath);
			m_currentScene->init();
			break;
		}
		case Scene::SceneResult::GoToMainMenu:
			m_currentScene = std::make_unique<gui::MainScreenScene>();
			m_currentScene->init();
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

void SceneManager::OnMouseButtonReleased(int32_t i_button)
{
	m_currentScene->OnMouseButtonReleased(i_button);
}

void SceneManager::ParseSceneConfigFilePath(const std::string& i_sceneConfigFilePath)
{
	std::ifstream file;
	file.open(i_sceneConfigFilePath);
	if (!file.is_open())
	{
		std::cerr << "No scene config file with path [" << i_sceneConfigFilePath << "] exists.\n";
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line.rfind("Level",0) == 0)
		{
			std::istringstream sstream(line);
			std::string levelText;
			std::getline(sstream, levelText, ' ');
			std::getline(sstream, levelText);
			uint32_t levelNumber = std::stoi(levelText);
			std::string tilemapPath;
			std::string physicsPath;
			std::getline(file, tilemapPath);
			std::getline(file, physicsPath);
			m_config.levels.emplace(levelNumber, LevelConfig(tilemapPath, physicsPath));
		}
	}

	m_currentScene = std::make_unique<gui::MainScreenScene>();
	m_currentScene->init();
	
}

SceneManager::LevelConfig::LevelConfig(const std::string& i_visualTilemapPath, const std::string& i_physicsTilemap)
	: visualTilemapPath(i_visualTilemapPath)
	, physicsMapPath(i_physicsTilemap)
{
}

}
}