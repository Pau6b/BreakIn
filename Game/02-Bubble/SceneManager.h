#pragma once
#include <string>
#include <vector>
#include <map>
#include "LevelScene.h"

namespace game
{
namespace core
{
class SceneManager
{
public:
	SceneManager(const std::string& i_sceneConfigFilePath);
private:

	void ParseSceneConfigFilePath(const std::string& i_sceneConfigFilePath);

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