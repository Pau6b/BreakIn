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
#include <map>
#include "Portal.h"
#include "GameplayHelpers.h"
#include "Ball.h"

//This is copied in LevelScene
#define SCREEN_X 32
#define SCREEN_Y 16

namespace game
{
namespace gameplay
{
namespace physics
{
CollisionManager::CollisionManager(const std::string& i_staticCollisionsPath,
								   const uint32_t i_tileSize,
								   uint32_t& i_currentMap,
								   const uint32_t i_currentMine,
								   const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks,
								   const std::vector<std::vector<std::shared_ptr<Coin>>>& i_coins,
								   const std::map<uint32_t,std::shared_ptr<BreakableBlock>>& i_keys,
								   const std::map<uint32_t, std::shared_ptr<Sensor>>& i_sensor,
								   std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> i_onBrokenBlockFunction,
								   std::function<void()> i_moveDown,
								   std::function<void()> i_moveUp,
								   core::CheatSystem& i_cheatSystem,
								   sound::SoundSystem& i_soundSystem,
								   visuals::ShaderProgram& i_shaderProgram)
	: m_tileSize(i_tileSize)
	, m_onBreakableBlockBroken(i_onBrokenBlockFunction)
	, m_cameraMoveDownFunction(i_moveDown)
	, m_cameraMoveUpFunction(i_moveUp)
	, m_cheatSystem(i_cheatSystem)
	, m_currentMap(&i_currentMap)
	, m_soundSystem(i_soundSystem)
	, m_sensor(i_sensor)
	, m_currentMine(i_currentMine)
	, m_shaderProgram(i_shaderProgram)
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
		if (m_staticCollisions[*m_currentMap][x][y] == "X" || m_staticCollisions[*m_currentMap][x][y] == "I")
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
		if (m_staticCollisions[*m_currentMap][x][y] == "X" ||  m_staticCollisions[*m_currentMap][x][y] == "I")
		{
			return CollisionResult::CollidedWithStaticBlock;
		}
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY)
{

	const int32_t originalY = ((i_pos.y + i_size.y - 1) / m_tileSize);
	const int32_t xini =  (i_pos.x) / m_tileSize;
	const int32_t xfin = (i_pos.x + i_size.x-1) / m_tileSize;
	const int32_t y = originalY%m_mapSizeY;
	const int32_t map_position = 2 - (originalY / m_mapSizeY);
	if (map_position != *m_currentMap)
	{
		*i_posY = m_tileSize * m_mapSizeY * (3-*m_currentMap) - i_size.y;
		return CollisionResult::CollidedWithScreen;
	}
	for (int32_t x = xini; x <= xfin; ++x)
	{
		if (m_staticCollisions[*m_currentMap][x][y] == "X" || m_staticCollisions[*m_currentMap][x][y] == "I")
		{
			*i_posY =  m_tileSize * m_mapSizeY * (2-*m_currentMap) + m_tileSize * y - i_size.y;
			return CollisionResult::CollidedWithStaticBlock;
		}
	}

	return CollisionResult::NoCollision;
}


CollisionResult CollisionManager::CollisionMoveUp(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY)
{
	const int32_t originalY =  ((i_pos.y + 1) / m_tileSize);
	const int32_t xini = (i_pos.x) / m_tileSize;
	const int32_t xfin = (i_pos.x + i_size.x-1) / m_tileSize;
	const int32_t y = originalY % m_mapSizeY;
	const int32_t map_position = 2 - (originalY / m_mapSizeY);
	if (map_position != *m_currentMap)
	{
		*i_posY = m_tileSize * m_mapSizeY * (2 - *m_currentMap);
		return CollisionResult::CollidedWithScreen;
	}
	for(int32_t x = xini; x <= xfin; ++x)
	{
		if (m_staticCollisions[*m_currentMap][x][y] == "X" || m_staticCollisions[*m_currentMap][x][y] == "I")
		{
			*i_posY = m_tileSize * m_mapSizeY * (2 - *m_currentMap) + m_tileSize * (y + 1);
			return CollisionResult::CollidedWithStaticBlock;
		}
	}
	return CollisionResult::NoCollision;
}


CollisionResult CollisionManager::CheckCollision(const int& i_posX, const int& i_posY)
{
	if (m_staticCollisions[*m_currentMap][i_posX][i_posY] != "0")
	{
		auto it = m_portalIds.find(m_staticCollisions[*m_currentMap][i_posX][i_posY]);
		if (m_staticCollisions[*m_currentMap][i_posX][i_posY] == "X" ||
			m_staticCollisions[*m_currentMap][i_posX][i_posY] == "Y" ||
			m_staticCollisions[*m_currentMap][i_posX][i_posY] == "I")
		{
			return CollisionResult::CollidedWithStaticBlock;
		}

		else if (m_staticCollisions[*m_currentMap][i_posX][i_posY] == "A")
		{
			return CollisionResult::CollidedWithAlarm;
		}
		else if(it != m_portalIds.end())
		{
			return CollisionResult::CollidedWithPortal;
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


bool CollisionManager::CollisionPlayer(const glm::vec2& i_pos, uint32_t i_size, float i_dirY, const float& i_Speed)
{
	glm::ivec2 posPlayer = m_player->GetPosition();
	glm::ivec2 sizePlayer = m_player->GetSize();

	uint32_t y_ini, y_end;
	y_ini = posPlayer.y;
	y_end = posPlayer.y + sizePlayer.y*0.4f;

	uint32_t ballDown = i_pos.y + i_size;
	uint32_t ballMid = i_pos.x + i_size / 2;

	return (ballDown >= y_ini && ballDown <= y_end && (ballMid >= posPlayer.x) && ballMid <= (posPlayer.x + sizePlayer.x+i_size/2) && i_dirY >= 0);
}

CollisionResult CollisionManager::CollisionBall(glm::vec2& i_pos, glm::vec2& i_dir, const int& i_size, const float i_speed, Ball* i_ball)
{
	int nsteps = 10;
	glm::vec2 originalPos = i_pos;
	for (int nstep = 1; nstep <= nsteps; nstep++)
	{
		bool isInPortal = false;
		const float currVel = i_speed*(float(nstep) / nsteps);
		const glm::vec2 new_pos = glm::vec2(originalPos.x + i_dir.x*currVel, originalPos.y + i_dir.y*currVel);
		const uint32_t x_modInTile = uint32_t((i_pos.x + (i_size / 2))) % m_tileSize;
		const uint32_t y_modInTile = uint32_t((i_pos.y + (i_size / 2))) % m_tileSize;
		const uint32_t x_mid = uint32_t((i_pos.x + (i_size / 2)) / m_tileSize) % m_staticCollisions[*m_currentMap][0].size();
		const uint32_t y_mid = uint32_t((i_pos.y + (i_size / 2)) / m_tileSize) % m_staticCollisions[*m_currentMap][0].size();
		const uint32_t x_right = uint32_t((new_pos.x + i_size) / m_tileSize) % m_staticCollisions[*m_currentMap][0].size();
		const uint32_t x_left = uint32_t((new_pos.x) / m_tileSize) % m_staticCollisions[*m_currentMap][0].size();
		const uint32_t y_upRaw = uint32_t(new_pos.y / m_tileSize);
		const uint32_t y_downRaw = uint32_t((new_pos.y + i_size + 1) / m_tileSize);
		const uint32_t y_up = y_upRaw % m_staticCollisions[*m_currentMap][0].size();
		const uint32_t y_down = y_downRaw %  m_staticCollisions[*m_currentMap][0].size();
		const uint32_t yDownMap = 2-(y_downRaw / m_mapSizeY);
		if (yDownMap != *m_currentMap)
		{
			uint32_t oldMap = *m_currentMap;
			auto it = m_sensor.find(*m_currentMap);
			if (it != m_sensor.end())
			{
				it->second->DesactivateAlarm();
			}
			m_cameraMoveDownFunction();
			if (oldMap > 0)
			{
				i_pos.y = (2 - *m_currentMap)*m_mapSizeY*m_tileSize+i_size;
				//#dani_todo #pau_todo quitar linea si no hay sonido
				//m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::LevelMoved);
			}
			return CollisionResult::CollidedWithScreen;
		}
		const uint32_t yUpMap = 2-(y_upRaw / m_mapSizeY);
		if (yUpMap != *m_currentMap)
		{
			auto it = m_sensor.find(*m_currentMap);
			if (it != m_sensor.end())
			{
				it->second->DesactivateAlarm();
			}
			m_cameraMoveUpFunction();
			i_pos.y = (3-*m_currentMap)*m_mapSizeY*m_tileSize-i_size-1;
			//m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::LevelMoved);
			return CollisionResult::CollidedWithScreen;
		}
		//Check up, down, left and right
		if (m_staticCollisions[*m_currentMap][x_mid][y_up] != "0" || m_staticCollisions[*m_currentMap][x_mid][y_down] != "0" ||
			m_staticCollisions[*m_currentMap][x_left][y_mid] != "0" || m_staticCollisions[*m_currentMap][x_right][y_mid] != "0")
		{
			uint32_t x, y, dir;
			std::tie(x, y, dir) = CheckDirectionOfCollision(x_mid, y_mid, x_right, x_left, y_up, y_down);
			CollisionResult collResult = CheckCollision(x, y);
			if (collResult == CollisionResult::CollidedWithPortal)
			{
				isInPortal = true;
				if (!i_ball->IsInPortal())
				{
					i_ball->SetInPortal(true);
					std::string portalId = m_staticCollisions[*m_currentMap][x][y];
					Portal* enteringPortal = m_portalIds.at(portalId).get();
					Portal* exitingPortal = m_portals.at(enteringPortal);
					i_pos = exitingPortal->GetPos();
					i_dir = helpers::CalculateExitingPortalDirection(i_dir, enteringPortal->GetReferenceVector(), exitingPortal->GetReferenceVector());
					m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::Portal);
					return CollisionResult::CollidedWithPortal;
				}
			}
			else
			{
				i_dir[dir] = -i_dir[dir];
				i_pos = originalPos + i_dir * 3.f;
				if (collResult == CollisionResult::CollidedWithStaticBlock)
				{
					m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BallStaticBlockCollision);
					return CollisionResult::CollidedWithStaticBlock;
				}
				else if (collResult == CollisionResult::CollidedWithAlarm)
				{
					m_sensor.at(*m_currentMap)->ActivateAlarm();
					return CollisionResult::CollidedWithAlarm;
				}
				else
				{
					ProcessBlockCollision(x, y);
					return CollisionResult::CollidedWithStaticBlock;
				}
			}

		}

		//Check diagonal that needs to be checked
		{
			uint32_t xDiag = x_mid;
			uint32_t yDiag = y_mid;
			uint32_t xSquare = 0;
			uint32_t ySquare = 0;
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
			glm::vec2 distBallCorner = glm::ivec2(x_modInTile - xSquare, y_modInTile - ySquare);
			float distBallCornerLength = glm::length(distBallCorner);
			if ( distBallCornerLength < i_size/2)
			{
				CollisionResult collisionResult = CheckCollision(xDiag, yDiag);
				if (collisionResult != CollisionResult::NoCollision)
				{
					if (collisionResult == CollisionResult::CollidedWithPortal)
					{
						isInPortal = true;
						if (!i_ball->IsInPortal())
						{
							i_ball->SetInPortal(true);
							std::string portalId = m_staticCollisions[*m_currentMap][xDiag][yDiag];
							Portal* enteringPortal = m_portalIds.at(portalId).get();
							Portal* exitingPortal = m_portals.at(enteringPortal);
							i_pos = exitingPortal->GetPos();
							i_dir = helpers::CalculateExitingPortalDirection(i_dir, enteringPortal->GetReferenceVector(), exitingPortal->GetReferenceVector());
							m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::Portal);
							return CollisionResult::CollidedWithPortal;
						}
					}
					else
					{
						CollisionResult yPartialCollision = CheckCollision(x_mid, yDiag);
						CollisionResult xPartialCollision = CheckCollision(xDiag, y_mid);
						i_dir = glm::normalize(distBallCorner);
						const float overPercentage = distBallCornerLength - (i_size / 2);
						const glm::vec2 correctingVector = -i_dir * (1.f / nsteps)* i_speed;
						i_pos = originalPos + 2.f*i_dir;
						if (collisionResult == CollisionResult::CollidedWithAlarm)
						{
							m_sensor.at(*m_currentMap)->ActivateAlarm();
							return CollisionResult::CollidedWithAlarm;
						}
						else if (collisionResult == CollisionResult::CollidedWithStaticBlock)
						{
							m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BallStaticBlockCollision);
						}
						else
						{
							ProcessBlockCollision(xDiag, yDiag);
							return collisionResult;
						}
						return CollisionResult::CollidedWithStaticBlock;
					}
				}
			}
		}

		if (CollisionPlayer(new_pos, i_size, i_dir[1], i_speed))
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
		if (!isInPortal)
		{
			i_ball->SetInPortal(false);
		}
	}
	return CollisionResult::NoCollision;

}

void CollisionManager::LinkPlayer(Player* i_player)
{
	m_player = i_player;
}

std::pair<uint32_t, uint32_t> CollisionManager::WipeDoorPositions()
{
	bool m_firstPos = true;
	std::pair<uint32_t, uint32_t> result;
	for (uint32_t i = 0; i < m_staticCollisions[*m_currentMap].size(); ++i)
	{
		if (m_staticCollisions[*m_currentMap][i][0] == "I")
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
			m_staticCollisions[*m_currentMap][i][0] = "0";
		}
	}
	return result;
}

std::vector<Portal*> CollisionManager::GetPortals() const
{
	std::vector<Portal*> portals;
	for (const auto& portal : m_portals)
	{
		portals.emplace_back(portal.first);
	}
	return portals;
}

void CollisionManager::DeleteKey(std::shared_ptr<BreakableBlock> i_key)
{
	const auto& it = std::find_if(std::begin(m_breakableBlocks[*m_currentMap]), std::end(m_breakableBlocks[*m_currentMap]), [i_key](const auto& i_pair) { return i_pair.second == i_key; });
	BreakIf(it == std::end(m_breakableBlocks[*m_currentMap]), "There is no key at this level");
	std::string id = std::to_string(it->first);
	for (uint32_t i = 0; i < m_staticCollisions[*m_currentMap].size(); ++i)
	{
		for (uint32_t j = 0; j < m_staticCollisions[*m_currentMap].size(); ++j)
		{
			if (m_staticCollisions[*m_currentMap][i][j] == id || m_staticCollisions[*m_currentMap][i][j] == "I")
			{
				m_staticCollisions[*m_currentMap][i][j] = "0";
			}
		}

	}
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
		if (resistanceLeft > 1)
		{
			m_soundSystem.PlayGameplaySounds(sound::GameplaySounds::BlockHit);
		}
		else if (resistanceLeft == 1)
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
	uint32_t blockPos = std::stoi(m_staticCollisions[*m_currentMap][i_x][i_y]);
	int32_t brickResistance = m_breakableBlocks[*m_currentMap].at(blockPos)->GetResistance() - 1;
	m_breakableBlocks[*m_currentMap].at(blockPos)->SetResistance(brickResistance);
	PlayBreakableBlockSound(m_breakableBlocks[*m_currentMap].at(blockPos));
	if (brickResistance <= 0)
	{
		m_onBreakableBlockBroken(m_breakableBlocks[*m_currentMap].at(blockPos));
		m_breakableBlocks[*m_currentMap].erase(blockPos);
		std::string blockId = m_staticCollisions[*m_currentMap][i_x][i_y];
		m_staticCollisions[*m_currentMap][i_x][i_y] = '0';
		for (uint32_t i = i_x - 1; i <= i_x + 1; ++i)
		{
			for(uint32_t j = i_y - 1; j <= i_y + 1; ++j)
			{
				if (m_staticCollisions[*m_currentMap][i][j] == blockId)
				{
					m_staticCollisions[*m_currentMap][i][j] = '0';
				}
			}
		}
	}
}


void CollisionManager::SetUpStaticCollisions(const std::string& i_staticCollisionMapPath,
											 const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks,
											 const std::vector<std::vector<std::shared_ptr<Coin>>>& i_coins,
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
		const uint32_t yOffset = (2 - i)*m_mapSizeY*m_tileSize;
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
				else if (c == 'K')
				{
					m_breakableBlocks[i].emplace(brickCounter, i_keys.at(i));
					m_breakableBlocks[i].at(brickCounter)->SetPosition(glm::vec2((k - 1)*m_tileSize, (j - 1)*m_tileSize + yOffset));
					std::string pos = std::to_string(brickCounter);
					brickCounter++;
					m_staticCollisions[i][k][j] = pos;
					m_staticCollisions[i][k - 1][j] = pos;
					m_staticCollisions[i][k][j - 1] = pos;
					m_staticCollisions[i][k - 1][j - 1] = pos;
				}
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
	uint32_t portalQuantity;
	fInput >> portalQuantity;
	uint32_t portalNum = 0;
	for (int i = 0; i < portalQuantity; ++i)
	{
		uint32_t portal1Map, portal1x, portal1y;
		char portal1Dir;
		fInput >> portal1Map >> portal1x >> portal1y >> portal1Dir;
		std::unique_ptr<Portal> portal1 = std::make_unique<Portal>(m_currentMine, helpers::CharToDirVector(portal1Dir), m_shaderProgram, glm::ivec2(SCREEN_X, SCREEN_Y));
		const uint32_t yOffset1 = (2 - portal1Map)*m_mapSizeY*m_tileSize;
		portal1->SetPosition(glm::vec2(portal1x*m_tileSize, portal1y*m_tileSize + yOffset1));
		uint32_t portal2Map, portal2x, portal2y;
		char portal2Dir;
		fInput >> portal2Map >> portal2x >> portal2y >> portal2Dir;
		std::unique_ptr<Portal> portal2 = std::make_unique<Portal>(m_currentMine, helpers::CharToDirVector(portal2Dir), m_shaderProgram, glm::ivec2(SCREEN_X, SCREEN_Y));
		const uint32_t yOffset2 = (2 - portal2Map)*m_mapSizeY*m_tileSize;
		portal2->SetPosition(glm::vec2(portal2x*m_tileSize, portal2y*m_tileSize + yOffset2));
		Portal* portal1Copy = portal1.get();
		Portal* portal2Copy = portal2.get();
		m_portals.emplace(portal1Copy, portal2Copy);
		m_portals.emplace(portal2Copy, portal1Copy);
		std::string p1Id = "P" + std::to_string(portalNum);
		portalNum++;
		m_portalIds.emplace(p1Id, std::move(portal1));
		m_staticCollisions[portal1Map][portal1x][portal1y] = p1Id;
		std::string p2Id = "P" + std::to_string(portalNum);
		portalNum++;
		m_portalIds.emplace(p2Id, std::move(portal2));
		m_staticCollisions[portal2Map][portal2x][portal2y] = p2Id;
	}
}

}
}
}
