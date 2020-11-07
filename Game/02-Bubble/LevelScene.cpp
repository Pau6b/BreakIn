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
#include "Sensor.h"
#include "Ball.h"
#include "Watcher.h"
#include "CheatSystem.h"
#include "Log.h"
#include "SoundSystem.h"
#include "Sprite.h"
#include "Text.h"

#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 11
#define INIT_PLAYER_Y_TILES 22

#define LEVEL_SIZE_X 456
#define LEVEL_SIZE_Y 384

#define FONT_SIZE 16

namespace game
{
namespace gameplay
{

LevelScene::LevelScene(const std::string& i_visualTilemapPath, const std::string& i_physicsMapPath, const core::CheatSystem& i_cheatSystem, sound::SoundSystem& i_soundSystem, uint32_t i_currentMine)
	: m_visualTilemapPath(i_visualTilemapPath)
	, m_physicsMapPath(i_physicsMapPath)
	, m_cheatSystem(i_cheatSystem)
	, m_currentMap(0)
	, m_soundSystem(i_soundSystem)
	, m_currentMine(i_currentMine)
{

}

LevelScene::~LevelScene()
{

}

void LevelScene::init()
{
	m_texProgram = std::make_unique<visuals::ShaderProgram>();
	InitShaders(*m_texProgram, "shaders/texture.vert", "shaders/texture.frag");
	m_map = std::make_unique<visuals::TileMap>(m_visualTilemapPath, glm::vec2(SCREEN_X, SCREEN_Y), *m_texProgram);
	m_map->SetCurrentMap(m_currentMap);
	ParseBricks(m_physicsMapPath);

	std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> onBreakableBlockBroken = [this](std::shared_ptr<BreakableBlock> i_brokenBlock) {
		OnBreakableBlockBroken(i_brokenBlock);
	};
	m_text = std::make_unique<gui::Text>("fonts/Minecraft-Regular.otf", FONT_SIZE);
	m_text->linkStr("LIVES: ", &m_currentLives);
	m_text->linkStr("ROOM: ", &m_currentMap);
	m_text->linkStr("MINE: ", &m_currentMine);
	m_collisionManager = std::make_unique<physics::CollisionManager>(m_physicsMapPath,
																	 m_map->getTileSize(),
																	 m_currentMap,
																	 m_bricks,
																	 m_coins,
																	 m_keys,
																	 m_sensor,
																	 onBreakableBlockBroken,
																	 std::bind(&LevelScene::MoveLevelDown, this),
																	 std::bind(&LevelScene::MoveLevelUp, this),
																	 m_cheatSystem,
																	 m_soundSystem);
	m_player = std::make_unique<Player>(*m_collisionManager);
	m_player->Init(glm::ivec2(SCREEN_X, SCREEN_Y), *m_texProgram,glm::vec2(INIT_PLAYER_X_TILES * m_map->getTileSize(), INIT_PLAYER_Y_TILES * m_map->getTileSize()), m_currentMap, LEVEL_SIZE_Y );

	std::for_each(std::begin(m_sensor), std::end(m_sensor), [Playerptr = m_player.get(), this](auto& it){  it.second->InitWatcher(*Playerptr, glm::ivec2(SCREEN_X, SCREEN_Y), *m_texProgram, std::bind(&LevelScene::LoseHP, this)); });

	m_collisionManager->LinkPlayer(m_player.get());

	m_ball = std::make_unique<Ball>(*m_collisionManager, *m_player, m_currentMap, glm::ivec2(SCREEN_X, SCREEN_Y), *m_texProgram,4);

	m_projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	m_currentTime = 0.0f;
	m_traslation = glm::mat4(1.0f);
	m_traslation[3][0] -= SCREEN_X - 40;
	m_projectionY = &m_traslation[3][1];
	*m_projectionY = -(SCREEN_Y)+48;
	*m_projectionY -= (2 - m_currentMap) * LEVEL_SIZE_Y;
	

}

void LevelScene::update(int i_deltaTime)
{
	if (core::Game::instance().getKey('p'))
	{
		Reset();
	}

	m_currentTime += i_deltaTime;
	m_player->Update(i_deltaTime);
	m_ball->Update(i_deltaTime);
	std::for_each(std::begin(m_bricks[m_currentMap]), std::end(m_bricks[m_currentMap]), [i_deltaTime](const std::shared_ptr<Brick>& i_brick) { i_brick->Update(i_deltaTime); });
	std::for_each(std::begin(m_coins[m_currentMap]), std::end(m_coins[m_currentMap]), [i_deltaTime](const std::shared_ptr<Coin>& i_coin) { i_coin->Update(i_deltaTime); });
	std::for_each(std::begin(m_blocksToCheck), std::end(m_blocksToCheck), [i_deltaTime](const std::shared_ptr<BreakableBlock>& i_breakable) { i_breakable->Update(i_deltaTime); });
	auto it = m_keys.find(m_currentMap);
	{
		if (it != m_keys.end())
		{
			it->second->Render();
		}
	}
	{
		auto it = m_sensor.find(m_currentMap);
		if (it != m_sensor.end())
		{
			it->second->Update(i_deltaTime);
		}
	}

}

void LevelScene::render()
{
	glm::mat4 modelview = glm::mat4(1.f);

	m_texProgram->use();
	m_texProgram->setUniformMatrix4f("projection", m_projection);
	m_texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);;
	m_texProgram->setUniformMatrix4f("modelview", modelview);
	m_texProgram->setUniformMatrix4f("model", m_traslation);
	m_texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
	m_map->render();
	m_blocksToCheck.erase(std::remove_if(std::begin(m_blocksToCheck), std::end(m_blocksToCheck), [](auto& i_breakable) {return i_breakable->GetResistance() == 0; }),
		std::end(m_blocksToCheck));
	std::for_each(std::begin(m_bricks[m_currentMap]), std::end(m_bricks[m_currentMap]), [](const std::shared_ptr<Brick>& i_brick) { i_brick->Render(); });
	std::for_each(std::begin(m_coins[m_currentMap]), std::end(m_coins[m_currentMap]), [](const std::shared_ptr<Coin>& i_coin) { i_coin->Render(); });
	std::for_each(std::begin(m_blocksToCheck), std::end(m_blocksToCheck), [](const std::shared_ptr<BreakableBlock>& i_breakable) { i_breakable->Render(); });
	auto it = m_keys.find(m_currentMap);
	{
		if (it != m_keys.end())
		{
			it->second->Render();
		}
	}
	{
		auto it = m_sensor.find(m_currentMap);
		if (it != m_sensor.end())
		{
			it->second->Render();
		}
	}
	m_player->Render();
	m_ball->Render();

	m_text->render();
}

void LevelScene::MoveLevelUp()
{
	*m_projectionY += LEVEL_SIZE_Y;
	m_currentMap++;
	m_player->SetCurrentMap(m_currentMap);
	m_collisionManager->SetCurrentMap(m_currentMap);
	m_map->SetCurrentMap(m_currentMap);
}

void LevelScene::MoveLevelDown()
{
	if (m_currentMap > 0)
	{
		*m_projectionY -= LEVEL_SIZE_Y;
		m_currentMap--;
		m_player->SetCurrentMap(m_currentMap);
		m_collisionManager->SetCurrentMap(m_currentMap);
		m_map->SetCurrentMap(m_currentMap);
	}
	else if (m_currentMap == 0)
	{
		LoseHP();
	}
}


std::pair<core::Scene::SceneResult, glm::uint32_t> LevelScene::GetSceneResult()
{
	return{ m_currentSceneResult, 0 };
}

void LevelScene::ParseBricks(std::string i_path)
{
	std::ifstream fInput;
	fInput.open(i_path);

	BreakIf(!fInput.is_open(), "Physics static collisions wrong path, path is : " + i_path);

	int sizex;

	std::stringstream sstream;
	std::string line;
	getline(fInput, line);
	sstream.str(line);
	sstream >> m_levelQuantity >> sizex >> m_levelSizeY;
	m_bricks = std::vector<std::unordered_set<std::shared_ptr<Brick>>>(m_levelQuantity);
	m_coins = std::vector<std::unordered_set<std::shared_ptr<Coin>>>(m_levelQuantity);
	m_sensor = std::map<uint32_t, std::shared_ptr<Sensor>>{};
	for (int i = m_levelQuantity - 1; i >= 0; --i)
	{
		glm::ivec2 doorPositions(-1,-1);
		for (int j = 0; j < m_levelSizeY; ++j)
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
					m_coins[i].emplace(std::make_shared<Coin>(*m_texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), CoinType::Iron));
				}
				else if (c == 'K')
				{
					BreakIf(m_keys.find(i) != m_keys.end(), "There is already a key for this level");
					m_keys.emplace( i,	std::make_shared<BreakableBlock>(1,
														std::make_unique<visuals::Sprite>(glm::vec2(32, 32),
																						  glm::vec2(1, 1),
																						  "images/Pickaxe.png",
																						  visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA,
																						  *m_texProgram),
														glm::ivec2(SCREEN_X, SCREEN_Y)));
				}
				else if (c == 'I')
				{
					if (doorPositions.x == -1)
					{
						doorPositions.x = k;
						doorPositions.y = k;
					}
					else
					{
						doorPositions.y = k;
					}
				}
				else if (c == 'A')
				{
					m_sensor.emplace(i, std::make_shared<Sensor>(std::make_unique<visuals::Sprite>(glm::vec2(32, 32), glm::vec2(1/7.f, 1), "images/luces.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGB, *m_texProgram),
													  glm::ivec2(SCREEN_X, SCREEN_Y)));
				}
			}
			//this is to clean the /n
			char temp;
			fInput.get(temp);
#ifndef _WIN32
			fin.get(temp);
#endif
		}
		if (doorPositions.x != -1)
		{
			int32_t nblocks = (doorPositions.y - doorPositions.x + 1);
			visuals::TileMap::BorderBlockInfo borderBlockInfo = m_map->GetBorderBlockInfo();
			std::unique_ptr<visuals::Sprite> doorSprite = std::make_unique<visuals::Sprite>(glm::vec2(16*nblocks,16),glm::vec2(nblocks,1),borderBlockInfo.texturePath,borderBlockInfo.texturePixelFormat, *m_texProgram);
			glm::vec2 doorPos(doorPositions.x * 16 + SCREEN_X, (2 - i)*m_levelSizeY * 16 + SCREEN_Y);
			doorSprite->setPosition(doorPos);
			m_map->SetDoorSprite(i, std::move(doorSprite));
		}
	}
}

void LevelScene::OnBreakableBlockBroken(std::shared_ptr<BreakableBlock> i_brokenBlock)
{
	std::shared_ptr<Brick> brick = std::dynamic_pointer_cast<Brick>(i_brokenBlock);
	std::shared_ptr<Coin> coin = std::dynamic_pointer_cast<Coin>(i_brokenBlock);
	if (brick)
	{
		m_text->UpdatePoints();
		m_blocksToCheck.emplace_back(i_brokenBlock);
		m_bricks[m_currentMap].erase(brick);
	}
	else if(coin)
	{
		m_text->UpdateCoins();
		m_blocksToCheck.emplace_back(i_brokenBlock);
		m_coins[m_currentMap].erase(coin);
		bool anyCoinLeft = false;
		for (uint32_t i = 0; i < m_coins.size() && !anyCoinLeft; ++i)
		{
			anyCoinLeft = m_coins[i].size() > 0;
		}
		if (!anyCoinLeft)
		{
			//m_currentSceneResult = core::Scene::SceneResult::GoToMainMenu;
		}
	}
	else //It's a key
	{
		m_keys.erase(m_currentMap);
		std::pair<uint32_t, uint32_t> wipedPositions = m_collisionManager->WipeDoorPositions();
		m_map->WipeDoorPositions(m_currentMap);
	}
}

void LevelScene::Reset()
{
	m_ball->Reset();
	m_player->Reset();
}

void LevelScene::LoseHP()
{
	if (m_currentLives == 0)
	{
		m_currentSceneResult = core::Scene::SceneResult::GoToMainMenu;
		m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::Died);
	}
	else
	{
		m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::LiveLost);
		Reset();
		m_currentLives--;
	}
}

}
}
