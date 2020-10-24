#include <iostream>
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <memory>
#include <functional>
#include "LevelScene.h"
#include "Game.h"
#include "CollisionManager.h"
#include "Player.h"
#include "TileMap.h"
#include "Brick.h"
#include "Coin.h"
#include "Ball.h"

#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10

#define LEVEL_SIZE_X 384
#define LEVEL_SIZE_Y 384

namespace game
{
namespace gameplay
{

LevelScene::LevelScene(const std::string& i_visualTilemapPath, const std::string& i_physicsMapPath)
	: m_visualTilemapPath(i_visualTilemapPath)
	, m_physicsMapPath(i_physicsMapPath)
{

}

LevelScene::~LevelScene()
{

}

void LevelScene::init()
{
	m_texProgram = std::make_unique<visuals::ShaderProgram>();
	InitShaders(*m_texProgram, "shaders/texture.vert", "shaders/texture.frag");
	ParseBricks(m_physicsMapPath);
	m_map = std::make_unique<visuals::TileMap>(m_visualTilemapPath, glm::vec2(SCREEN_X, SCREEN_Y), *m_texProgram);

	std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> onBreakableBlockBroken = [this](std::shared_ptr<BreakableBlock> i_brokenBlock) {
		OnBreakableBlockBroken(i_brokenBlock);
	};

	m_collisionManager = std::make_unique<physics::CollisionManager>(m_physicsMapPath, m_map->getTileSize(), m_bricks, m_coins, onBreakableBlockBroken, std::bind(&LevelScene::MoveLevelDown, this), std::bind(&LevelScene::MoveLevelUp, this));
	m_player = std::make_unique<Player>(*m_collisionManager);
	m_player->init(glm::ivec2(SCREEN_X, SCREEN_Y), *m_texProgram);
	m_player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * m_map->getTileSize(), INIT_PLAYER_Y_TILES * m_map->getTileSize()));

	m_collisionManager->LinkPlayer(m_player.get());

	m_ball = std::make_unique<Ball>(*m_collisionManager);
	m_ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), *m_texProgram);
	m_ball->setPosition(glm::vec2((7 + INIT_PLAYER_X_TILES) * m_map->getTileSize(), (7 + INIT_PLAYER_Y_TILES) * m_map->getTileSize()));

	m_projection = glm::ortho(0.f, float(LEVEL_SIZE_X - 1 + 160), float(LEVEL_SIZE_Y - 1), 0.f);
	m_currentTime = 0.0f;
	m_traslation = glm::mat4(1.0f);
	m_traslation[3][0] -= SCREEN_X - 10;
	m_current_level = &m_traslation[3][1];
	*m_current_level = -SCREEN_Y;
}

void LevelScene::update(int i_deltaTime)
{
	m_currentTime += i_deltaTime;
	m_player->update(i_deltaTime);
	m_ball->update(i_deltaTime);
}

void LevelScene::render()
{
	glm::mat4 modelview;

	m_texProgram->use();
	m_texProgram->setUniformMatrix4f("projection", m_projection);
	m_texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	m_texProgram->setUniformMatrix4f("modelview", modelview);
	m_texProgram->setUniformMatrix4f("model", m_traslation);
	m_texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
	m_map->render();
	m_player->render();
	std::for_each(std::begin(m_bricks[m_currentMap]), std::end(m_bricks[m_currentMap]), [](const std::shared_ptr<Brick>& i_brick) { i_brick->Render(); });
	std::for_each(std::begin(m_coins[m_currentMap]), std::end(m_coins[m_currentMap]), [](const std::shared_ptr<Coin>& i_coin) { i_coin->Render(); });
	m_ball->render();
}

void LevelScene::MoveLevelUp()
{
	*m_current_level += LEVEL_SIZE_Y;
}

void LevelScene::MoveLevelDown()
{
	*m_current_level -= LEVEL_SIZE_Y;
}


std::pair<core::Scene::SceneResult, glm::uint32_t> LevelScene::GetSceneResult()
{
	return{ m_currentSceneResult, 0 };
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
	m_bricks = std::vector<std::unordered_set<std::shared_ptr<Brick>>>(levelQuantity);
	m_coins = std::vector<std::unordered_set<std::shared_ptr<Coin>>>(levelQuantity);
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
					m_bricks[i].emplace(std::make_shared<Brick>(*m_texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), c - '0'));
				}
				else if (c == 'N')
				{
					m_coins[i].emplace(std::make_shared<Coin>(*m_texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), CoinType::Diamond));
				}
				else if (c == 'M')
				{
					m_coins[i].emplace(std::make_shared<Coin>(*m_texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), CoinType::Emerald));
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

void LevelScene::OnBreakableBlockBroken(std::shared_ptr<BreakableBlock> i_brokenBlock)
{
	std::shared_ptr<Brick> brick = std::dynamic_pointer_cast<Brick>(i_brokenBlock);
	std::shared_ptr<Coin> coin = std::dynamic_pointer_cast<Coin>(i_brokenBlock);
	if (brick)
	{
		m_bricks[m_currentMap].erase(brick);
	}
	else if(coin)
	{
		m_coins[m_currentMap].erase(coin);
		bool anyCoinLeft = false;
		for (uint32_t i = 0; i < m_coins.size() && !anyCoinLeft; ++i)
		{
			anyCoinLeft = m_coins[i].size() > 0;
		}
		if (!anyCoinLeft)
		{
			m_currentSceneResult = core::Scene::SceneResult::GoToMainMenu;
		}
	}
}

}
}
