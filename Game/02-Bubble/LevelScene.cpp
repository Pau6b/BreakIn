#include <iostream>
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include "LevelScene.h"
#include "Game.h"
#include "CollisionManager.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10

namespace game
{
namespace gameplay
{

LevelScene::LevelScene(const std::string& i_visualTilemapPath, const std::string& i_physicsMapPath)
	: m_visualTilemapPath(i_visualTilemapPath)
	, m_physicsMapPath(i_physicsMapPath)
{

}

void LevelScene::init()
{
	m_texProgram = std::make_unique<visuals::ShaderProgram>();
	InitShaders(*m_texProgram, "shaders/texture.vert", "shaders/texture.frag");
	ParseBricks(m_physicsMapPath);
	m_map = std::make_unique<TileMap>(m_visualTilemapPath, glm::vec2(SCREEN_X, SCREEN_Y), *m_texProgram);
	m_collisionManager = std::make_unique<physics::CollisionManager>(m_physicsMapPath, m_map->getTileSize(), m_bricks);
	m_player = std::make_unique<Player>(*m_collisionManager);
	m_player->init(glm::ivec2(SCREEN_X, SCREEN_Y), *m_texProgram);
	m_player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * m_map->getTileSize(), INIT_PLAYER_Y_TILES * m_map->getTileSize()));
	m_projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	m_currentTime = 0.0f;
}

void LevelScene::update(int i_deltaTime)
{
	m_currentTime += i_deltaTime;
	m_player->update(i_deltaTime);
}

void LevelScene::render()
{
	glm::mat4 modelview;

	m_texProgram->use();
	m_texProgram->setUniformMatrix4f("projection", m_projection);
	m_texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	m_texProgram->setUniformMatrix4f("modelview", modelview);
	m_texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
	m_map->render();
	m_player->render();
	m_bricks[m_currentMap].erase(std::remove_if(begin(m_bricks[m_currentMap]), end(m_bricks[m_currentMap]), [](const std::shared_ptr<Brick>& i_brick) { return i_brick->GetResistance() == 0; }),
		end(m_bricks[m_currentMap]));
	std::for_each(begin(m_bricks[m_currentMap]), end(m_bricks[m_currentMap]), [](const std::shared_ptr<Brick>& i_brick) { i_brick->Render(); });
}

std::pair<core::Scene::SceneResult, glm::uint32_t> LevelScene::GetSceneResult()
{
	return{ core::Scene::SceneResult::NotFinished, 0 };
}

void LevelScene::ParseBricks(std::string i_path)
{
	std::ifstream fInput;
	fInput.open(i_path);

	if (!fInput.is_open())
	{
		std::cerr << "Physics static collisions wrong path, path is : " << i_path;
		return;
	}

	int levelQuantity;
	int sizex;
	int sizey;

	std::stringstream sstream;
	std::string line;
	getline(fInput, line);
	sstream.str(line);
	sstream >> levelQuantity >> sizex >> sizey;
	m_bricks = std::vector<std::vector<std::shared_ptr<Brick>>>(levelQuantity);
	for (int i = levelQuantity - 1; i >= 0; --i)
	{
		for (int j = 0; j < sizey; ++j)
		{
			for (int k = 0; k < sizex; ++k)
			{
				char c;
				fInput.get(c);
				if (c == '1' || c == '2' || c == '3')
				{
					m_bricks[i].emplace_back(std::make_shared<Brick>(*m_texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), c - '0'));
				}
			}
			//this is to clean the /n
			char temp;
			fInput.get(temp);
#ifndef _WIN32
			fin.get(temp);
#endif
		}
	}
}
}
}

