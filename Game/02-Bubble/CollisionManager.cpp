#include "CollisionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "BreakableBlock.h"
#include "Brick.h"
#include "Coin.h"
#include "Player.h"
#include "Sensor.h"
#include "glm\detail\func_geometric.hpp"
#include "CheatSystem.h"
#include "Log.h"
#include "SoundSystem.h"
#include "Sounds.h"
#include <memory>

namespace game
{
namespace gameplay
{
namespace physics
{
CollisionManager::CollisionManager(const std::string& i_staticCollisionsPath,
								   const uint32_t i_tileSize,
								   uint32_t i_currentMap,
								   const std::vector<std::unordered_set<std::shared_ptr<Brick>>>& i_bricks,
								   const std::vector<std::unordered_set<std::shared_ptr<Coin>>>& i_coins,
								   const std::map<uint32_t,std::shared_ptr<BreakableBlock>>& i_keys,
								   const std::map<uint32_t, std::shared_ptr<Sensor>>& i_sensor,
								   std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> i_onBrokenBlockFunction,
								   std::function<void()> i_moveDown,
								   std::function<void()> i_moveUp,
								   const core::CheatSystem& i_cheatSystem,
								   sound::SoundSystem& i_soundSystem)
	: m_tileSize(i_tileSize)
	, m_onBreakableBlockBroken(i_onBrokenBlockFunction)
	, m_cameraMoveDownFunction(i_moveDown)
	, m_cameraMoveUpFunction(i_moveUp)
	, m_cheatSystem(i_cheatSystem)
	, m_currentMap(i_currentMap)
	, m_soundSystem(i_soundSystem)
	, m_sensor(i_sensor)

{
	SetUpStaticCollisions(i_staticCollisionsPath, i_bricks, i_coins, i_keys);
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.


CollisionResult CollisionManager::CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size)
{
	int32_t x = i_pos.x / m_tileSize;
	int32_t y0 =  (i_pos.y / m_tileSize) % m_mapSizeY;
	int32_t y1 =  ((i_pos.y + i_size.y - 1) / m_tileSize) % m_mapSizeY;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] == "X" || m_staticCollisions[m_currentMap][x][y] == "I")
		{
			return CollisionResult::CollidedWithStaticBlock;
		}
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::CollisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size)
{
	const int32_t x = (i_pos.x + i_size.x - 1) / m_tileSize;
	const int32_t y0 =  (i_pos.y / m_tileSize) % m_mapSizeY;
	const int32_t y1 =  ((i_pos.y + i_size.y - 1) / m_tileSize) % m_mapSizeY;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] == "X" ||  m_staticCollisions[m_currentMap][x][y] == "I")
		{
			return CollisionResult::CollidedWithStaticBlock;
		}
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY)
{

	const int32_t originalY = ((i_pos.y + i_size.y - 1) / m_tileSize);
	const int32_t x =  (i_pos.x + i_size.x/2) / m_tileSize;
	const int32_t y = originalY%m_mapSizeY;
	const int32_t map_position = 2 - (originalY / m_mapSizeY);
	if (map_position != m_currentMap)
	{
		*i_posY = m_tileSize * m_mapSizeY * (3-m_currentMap) - i_size.y;
		return CollisionResult::CollidedWithScreen;
	}
	else if (m_staticCollisions[m_currentMap][x][y] == "X" || m_staticCollisions[m_currentMap][x][y] == "I")
	{
		*i_posY = m_tileSize * y - i_size.y;
		return CollisionResult::CollidedWithStaticBlock;
	}

	return CollisionResult::NoCollision;
}


CollisionResult CollisionManager::CollisionMoveUp(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY)
{
	const int32_t originalY =  ((i_pos.y + 1) / m_tileSize);
	const int32_t x = (i_pos.x + i_size.x/2) / m_tileSize;
	const int32_t y = originalY % m_mapSizeY;
	const int32_t map_position = 2 - (originalY / m_mapSizeY);
	if (map_position != m_currentMap)
	{
		*i_posY = m_tileSize * m_mapSizeY * (2 - m_currentMap);
		return CollisionResult::CollidedWithScreen;
	}
	if (m_staticCollisions[m_currentMap][x][y] == "X" || m_staticCollisions[m_currentMap][x][y] == "I")
	{
		*i_posY = m_tileSize * (y + 1);
		return CollisionResult::CollidedWithStaticBlock;
	}
	return CollisionResult::NoCollision;
}


CollisionResult CollisionManager::CheckCollision(const int& i_posX, const int& i_posY)
{
	if (m_staticCollisions[m_currentMap][i_posX][i_posY] != "0")
	{
		if (m_staticCollisions[m_currentMap][i_posX][i_posY] == "X" ||
			m_staticCollisions[m_currentMap][i_posX][i_posY] == "Y" ||
			m_staticCollisions[m_currentMap][i_posX][i_posY] == "I")
		{
			return CollisionResult::CollidedWithStaticBlock;
		}

		else if (m_staticCollisions[m_currentMap][i_posX][i_posY] == "A")
		{
			return CollisionResult::CollidedWithAlarm;
		}

		else
		{
			return CollisionResult::CollidedWithBrick;
		}
	}

	return CollisionResult::NoCollision;
}


std::tuple<uint32_t,uint32_t,uint32_t> CollisionManager::CheckDirectionOfCollision(const int& i_XposMid, const int& i_YposMid, const int& i_XposRight, const int& i_XposLeft, const int& i_YposUp, const int& i_YposDown)
{
	//Check Up
	if (CheckCollision(i_XposMid, i_YposUp) != CollisionResult::NoCollision)
	{
		return std::make_tuple(i_XposMid, i_YposUp, 1);
	}
	//Check Down
	else if (CheckCollision(i_XposMid, i_YposDown) != CollisionResult::NoCollision)
	{
		return std::make_tuple(i_XposMid, i_YposDown, 1);
	}
	//Check Right
	else if (CheckCollision(i_XposRight, i_YposMid) != CollisionResult::NoCollision)
	{
		return std::make_tuple(i_XposRight, i_YposMid, 0);
	}
	//Check Left
	else if (CheckCollision(i_XposLeft, i_YposMid) != CollisionResult::NoCollision)
	{
		return std::make_tuple(i_XposLeft, i_YposMid, 0);
	}

	return std::make_tuple(-1, -1, -1);

}


bool CollisionManager::CollisionPlayer(const glm::vec2& i_pos, uint32_t i_size, float i_dirY, const int32_t& i_Speed)
{
	glm::ivec2 posPlayer = m_player->GetPosition();
	glm::ivec2 sizePlayer = m_player->GetSize();

	uint32_t y_ini, y_end;
	y_ini = posPlayer.y;
	y_end = posPlayer.y + sizePlayer.y;

	uint32_t ballDown = i_pos.y + i_size;
	uint32_t ballMid = i_pos.x + i_size / 2;

	return (ballDown >= y_ini && ballDown <= y_end && (ballMid >= posPlayer.x) && ballMid <= (posPlayer.x + sizePlayer.x+i_size/2) && i_dirY > 0);
}

CollisionResult CollisionManager::CollisionBall(glm::vec2& i_pos, glm::vec2& i_dir, const int& i_size, const float& i_speed)
{
	int nsteps = 10;
	glm::vec2 originalPos = i_pos;
	for (int nstep = 1; nstep <= nsteps; nstep++)
	{
		const float currVel = i_speed*(float(nstep) / nsteps);
		const glm::vec2 new_pos = glm::vec2(originalPos.x + i_dir.x*currVel, originalPos.y + i_dir.y*currVel);
		const uint32_t x_modInTile = uint32_t((i_pos.x + (i_size / 2))) % m_tileSize;
		const uint32_t y_modInTile = uint32_t((i_pos.y + (i_size / 2))) % m_tileSize;
		const uint32_t x_mid = uint32_t((i_pos.x + (i_size / 2)) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		const uint32_t y_mid = uint32_t((i_pos.y + (i_size / 2)) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		const uint32_t x_right = uint32_t((new_pos.x + i_size) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		const uint32_t x_left = uint32_t((new_pos.x) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		const uint32_t y_upRaw = uint32_t(new_pos.y / m_tileSize);
		const uint32_t y_downRaw = uint32_t((new_pos.y + i_size + 1) / m_tileSize);
		const uint32_t y_up = y_upRaw % m_staticCollisions[m_currentMap][0].size();
		const uint32_t y_down = y_downRaw %  m_staticCollisions[m_currentMap][0].size();
		const uint32_t yDownMap = 2-(y_downRaw / m_mapSizeY);
		if (yDownMap != m_currentMap)
		{
			uint32_t oldMap = m_currentMap;
			auto it = m_sensor.find(m_currentMap);
			if (it != m_sensor.end())
			{
				it->second->DesactivateAlarm();
			}
			m_cameraMoveDownFunction();
			if (oldMap > 0)
			{
				i_pos.y = (2 - m_currentMap)*m_mapSizeY*m_tileSize+i_size;
				m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::LevelMoved);
			}
			return CollisionResult::CollidedWithScreen;
		}
		const uint32_t yUpMap = 2-(y_upRaw / m_mapSizeY);
		if (yUpMap != m_currentMap)
		{
			auto it = m_sensor.find(m_currentMap);
			if (it != m_sensor.end())
			{
				it->second->DesactivateAlarm();
			}
			m_cameraMoveUpFunction();
			i_pos.y = (3-m_currentMap)*m_mapSizeY*m_tileSize-i_size-1;
			m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::LevelMoved);
			return CollisionResult::CollidedWithScreen;
		}
		//Check up, down, left and right
		if (m_staticCollisions[m_currentMap][x_mid][y_up] != "0" || m_staticCollisions[m_currentMap][x_mid][y_down] != "0" ||
			m_staticCollisions[m_currentMap][x_left][y_mid] != "0" || m_staticCollisions[m_currentMap][x_right][y_mid] != "0")
		{
			uint32_t x, y, dir;
			std::tie(x, y, dir) = CheckDirectionOfCollision(x_mid, y_mid, x_right, x_left, y_up, y_down);

			if (CheckCollision(x, y) == CollisionResult::CollidedWithStaticBlock)
			{
				i_dir[dir] = -i_dir[dir];
				m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BallStaticBlockCollision);
				return CollisionResult::CollidedWithStaticBlock;
			}
			else if (CheckCollision(x, y) == CollisionResult::CollidedWithAlarm)
			{
				i_dir[dir] = -i_dir[dir];
				m_sensor.at(m_currentMap)->ActivateAlarm();
				return CollisionResult::CollidedWithAlarm;
			}
			else
			{
				i_dir[dir] = -i_dir[dir];
				ProcessBlockCollision(x, y);
				return CollisionResult::CollidedWithStaticBlock;
			}

		}

		//Check diagonal that needs to be checked
		{
			int32_t xDiag = x_mid;
			int32_t yDiag = y_mid;
			int32_t xSquare = 0;
			int32_t ySquare = 0;
			if (x_modInTile > m_tileSize/2)
			{
				xDiag++;
				xSquare = m_tileSize;
			}
			else
			{
				xDiag--;
			}
			if (y_modInTile > m_tileSize/2)
			{
				yDiag++;
				ySquare = m_tileSize;
			}
			else
			{
				yDiag--;
			}
			if (glm::length(glm::vec2(x_modInTile-xSquare, y_modInTile-ySquare)) < i_size/2)
			{
				CollisionResult collisionResult = CheckCollision(xDiag, yDiag);
				if (collisionResult != CollisionResult::NoCollision)
				{
					if (x_modInTile > y_modInTile)
					{
						i_dir.x = -i_dir.x;
					}
					else
					{
						i_dir.y = -i_dir.y;
					}

					if (collisionResult != CollisionResult::CollidedWithStaticBlock)
					{
						ProcessBlockCollision(xDiag, yDiag);
					}
					else if (collisionResult == CollisionResult::CollidedWithStaticBlock)
					{
						m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BallStaticBlockCollision);
					}
					return CollisionResult::CollidedWithStaticBlock;
				}
			}
		}

		if (i_pos.y + i_size < m_player->GetPosition().y && CollisionPlayer(new_pos, i_size, i_dir[1], i_speed))
		{
			if (m_cheatSystem.InGodMode())
			{
				i_dir = glm::vec2(0, -1);
			}
			else
			{
				glm::vec2 newDirNoChange = i_dir;
				newDirNoChange[1] = -newDirNoChange[1];

				glm::vec2 ballDownPos = glm::vec2(new_pos.x + i_size / 2, new_pos.y);
				glm::vec2 playerPos = m_player->GetPosition();
				playerPos.x += m_player->GetSize().x / 2.f;
				glm::vec2 playerToBall = glm::vec2(ballDownPos.x - playerPos.x, ballDownPos.y - playerPos.y);
				playerToBall = glm::normalize(playerToBall);

				i_dir = glm::normalize(playerToBall*0.7f + newDirNoChange*0.3f);
				i_pos.y = m_player->GetPosition().y - i_size;
			}
			m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BallBarCollision);
			return CollisionResult::CollidedWithPlayer;
		}

		i_pos = new_pos;
	}

	return CollisionResult::NoCollision;

}

void CollisionManager::LinkPlayer(Player* i_player)
{
	m_player = i_player;
}

void CollisionManager::SetCurrentMap(uint32_t i_currentMap)
{
	m_currentMap = i_currentMap;
}

std::pair<uint32_t, uint32_t> CollisionManager::WipeDoorPositions()
{
	bool m_firstPos = true;
	std::pair<uint32_t, uint32_t> result;
	for (uint32_t i = 0; i < m_staticCollisions[m_currentMap].size(); ++i)
	{
		if (m_staticCollisions[m_currentMap][i][0] == "I")
		{
			if (m_firstPos)
			{
				m_firstPos = false;
				result.first = i;
			}
			else
			{
				result.second = i;
			}
			m_staticCollisions[m_currentMap][i][0] = "0";
		}
	}
	return result;
}

void CollisionManager::PlayBreakableBlockSound(std::shared_ptr<BreakableBlock> i_breakableBlock)
{
	Coin* coin = dynamic_cast<Coin*>(i_breakableBlock.get());
	Brick* brick = dynamic_cast<Brick*>(i_breakableBlock.get());
	if (coin)
	{
		m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BallCoin);
	}
	else if (brick)
	{
		uint32_t resistanceLeft = i_breakableBlock->GetResistance();
		if (resistanceLeft > 0)
		{
			m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BlockHit);
		}
		else if (resistanceLeft == 0)
		{
			m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BlockBroken);
		}
	}
	else //It's a key
	{
		m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BallKey);
	}
}

void CollisionManager::ProcessBlockCollision(uint32_t i_x, uint32_t i_y)
{
	uint32_t blockPos = std::stoi(m_staticCollisions[m_currentMap][i_x][i_y]);
	uint32_t brickResistance = m_breakableBlocks[m_currentMap].at(blockPos)->GetResistance() - 1;
	m_breakableBlocks[m_currentMap].at(blockPos)->SetResistance(brickResistance);
	PlayBreakableBlockSound(m_breakableBlocks[m_currentMap].at(blockPos));
	if (brickResistance == 0)
	{
		m_onBreakableBlockBroken(m_breakableBlocks[m_currentMap].at(blockPos));
		m_breakableBlocks[m_currentMap].erase(blockPos);
		std::string blockId = m_staticCollisions[m_currentMap][i_x][i_y];
		m_staticCollisions[m_currentMap][i_x][i_y] = '0';
		for (uint32_t i = i_x - 1; i <= i_x + 1; ++i)
		{
			for(uint32_t j = i_y - 1; j <= i_y + 1; ++j)
			{
				if (m_staticCollisions[m_currentMap][i][j] == blockId)
				{
					m_staticCollisions[m_currentMap][i][j] = '0';
				}
			}
		}
	}
}


void CollisionManager::SetUpStaticCollisions(const std::string& i_staticCollisionMapPath,
											 const std::vector<std::unordered_set<std::shared_ptr<Brick>>>& i_bricks,
											 const std::vector<std::unordered_set<std::shared_ptr<Coin>>>& i_coins,
											 const std::map<uint32_t, std::shared_ptr<BreakableBlock>>& i_keys)
{
	std::ifstream fInput;
	fInput.open(i_staticCollisionMapPath);

	BreakIf(!fInput.is_open(), "Physics static collisions wrong path, path is : " + i_staticCollisionMapPath);

	int levelQuantity;
	int sizex;

	std::stringstream sstream;
	std::string line;
	std::getline(fInput, line);
	sstream.str(line);
	sstream >> levelQuantity >> sizex >> m_mapSizeY;
	m_staticCollisions = std::vector<Matrix<std::string>>(levelQuantity, Matrix<std::string>(m_mapSizeY, Row<std::string>(sizex)));
	uint32_t brickCounter = 1;
	m_breakableBlocks = std::vector<std::map<uint32_t,std::shared_ptr<BreakableBlock>>>(levelQuantity);
	for (int32_t i = levelQuantity-1; i >= 0; --i)
	{
		auto blocksIt = i_bricks[i].begin();
		auto coinsIt = i_coins[i].begin();
		const uint32_t yOffset = (2 - m_currentMap)*m_mapSizeY*m_tileSize;
		for (int j = 0; j < m_mapSizeY; ++j)
		{
			for (int k = 0; k < sizex; ++k)
			{
				char c;
				fInput.get(c);
				if (c == '1' || c == '2' || c == '3')
				{
					m_breakableBlocks[i].emplace(brickCounter, (*blocksIt));
					++blocksIt;
					m_breakableBlocks[i].at(brickCounter)->SetPosition(glm::vec2((k-1)*m_tileSize, j*m_tileSize+yOffset));
					std::string pos = std::to_string(brickCounter);
					brickCounter++;
					m_staticCollisions[i][k][j] = pos;
					m_staticCollisions[i][k-1][j] = pos;
				}
				else if(c == 'N' || c == 'M')
				{
					m_breakableBlocks[i].emplace(brickCounter, (*coinsIt));
					++coinsIt;
					m_breakableBlocks[i].at(brickCounter)->SetPosition(glm::vec2((k - 1)*m_tileSize, (j-1)*m_tileSize+yOffset));
					std::string pos = std::to_string(brickCounter);
					brickCounter++;
					m_staticCollisions[i][k][j] = pos;
					m_staticCollisions[i][k - 1][j] = pos;
 					m_staticCollisions[i][k][j-1] = pos;
 					m_staticCollisions[i][k - 1][j-1] = pos;
				}
				else if(c == 'K')
				{
					m_breakableBlocks[i].emplace(brickCounter, i_keys.at(i));
					m_breakableBlocks[i].at(brickCounter)->SetPosition(glm::vec2((k - 1)*m_tileSize, (j - 1)*m_tileSize+yOffset));
					std::string pos = std::to_string(brickCounter);
					brickCounter++;
					m_staticCollisions[i][k][j] = pos;
					m_staticCollisions[i][k - 1][j] = pos;
					m_staticCollisions[i][k][j - 1] = pos;
					m_staticCollisions[i][k - 1][j - 1] = pos;
				else if (c == 'A')
				{
					m_sensor.at(i)->SetPosition(glm::vec2((k - 1)*m_tileSize, (j - 1)*m_tileSize));
					m_staticCollisions[i][k][j] = "A";
					m_staticCollisions[i][k - 1][j] = "A";
					m_staticCollisions[i][k][j - 1] = "A";
					m_staticCollisions[i][k - 1][j - 1] = "A";
				}
				else
				{
					m_staticCollisions[i][k][j] = c;
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
}
