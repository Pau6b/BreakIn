#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "LevelScene.h"

namespace game
{
namespace core
{
SceneManager::SceneManager(const std::string& i_sceneConfigFilePath)
{
	ParseSceneConfigFilePath(i_sceneConfigFilePath);
}

void SceneManager::update(int i_deltaTime)
{
	if (m_currentScene)
	{
		m_currentScene->update(i_deltaTime);
	}
}

void SceneManager::render()
{
	if (m_currentScene)
	{
		m_currentScene->render();
	}
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

	//#pau_todo #dani_todo this is hardcoded in order to have a scene now, change it when we have to load diferent scenes
	m_currentScene = std::make_unique<gameplay::LevelScene>(m_config.levels.at(1).visualTilemapPath, m_config.levels.at(1).physicsMapPath);
	m_currentScene->init();
	
}

SceneManager::LevelConfig::LevelConfig(const std::string& i_visualTilemapPath, const std::string& i_physicsTilemap)
	: visualTilemapPath(i_visualTilemapPath)
	, physicsMapPath(i_physicsTilemap)
{
}

}
}