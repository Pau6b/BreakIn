#include "CollisionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "BreakableBlock.h"
#include "Brick.h"
#include "Coin.h"
#include "Player.h"
#include "glm\detail\func_geometric.hpp"

namespace game
{
namespace gameplay
{
namespace physics
{
CollisionManager::CollisionManager(const std::string& i_staticCollisionsPath,
								   const uint32_t i_tileSize,
								   const std::vector<std::unordered_set<std::shared_ptr<Brick>>>& i_bricks,
								   const std::vector<std::unordered_set<std::shared_ptr<Coin>>>& i_coins,
								   std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> i_onBrokenBlockFunction,
								   std::function<void()> i_moveDown,
									 std::function<void()> i_moveUp)
	: m_tileSize(i_tileSize)
	, m_onBreakableBlockBroken(i_onBrokenBlockFunction)
	, m_cameraMoveDownFunction(i_moveDown)
	, m_cameraMoveUpFunction(i_moveUp)
{
	SetUpStaticCollisions(i_staticCollisionsPath, i_bricks, i_coins);
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.


CollisionResult CollisionManager::CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size)
{
	int32_t x, y0, y1;

	x = i_pos.x / m_tileSize;
	y0 =  (i_pos.y / m_tileSize) % m_mapSizeY;
	y1 =  ((i_pos.y + i_size.y - 1) / m_tileSize) % m_mapSizeY;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] != "0")
		{
			if (m_staticCollisions[m_currentMap][x][y] == "X"
				|| m_staticCollisions[m_currentMap][x][y] == "I")
			{
				return CollisionResult::CollidedWithStaticBlock;
			}
			else
			{
				ProcessBlockCollision(x, y);
				return CollisionResult::CollidedWithBrick;
			}
		}
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::CollisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size)
{
	int32_t x, y0, y1;

	x = (i_pos.x + i_size.x - 1) / m_tileSize;
	y0 =  (i_pos.y / m_tileSize) % m_mapSizeY;
	y1 =  ((i_pos.y + i_size.y - 1) / m_tileSize) % m_mapSizeY;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] != "0")
		{
			if (m_staticCollisions[m_currentMap][x][y] == "X"
				|| m_staticCollisions[m_currentMap][x][y] == "I")
			{
				return CollisionResult::CollidedWithStaticBlock;
			}
			else
			{
				ProcessBlockCollision(x, y);
				return CollisionResult::CollidedWithBrick;
			}
		}
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY)
{
	int32_t x, y;
	y = ((i_pos.y + i_size.y - 1) / m_tileSize) % m_mapSizeY;
	x =  (i_pos.x + i_size.x/2) / m_tileSize;
	if (y >= m_staticCollisions[m_currentMap][x].size())
	{
		*i_posY = m_tileSize * y - i_size.y;
		return CollisionResult::CollidedWithScreen;
	}
	else if (m_staticCollisions[m_currentMap][x][y] == "0")
	{
		return CollisionResult::NoCollision;
	}
	else
	{
		*i_posY = m_tileSize * y - i_size.y;
		if (m_staticCollisions[m_currentMap][x][y] == "X"
			|| m_staticCollisions[m_currentMap][x][y] == "I")
		{
			return CollisionResult::CollidedWithStaticBlock;
		}
		else
		{
			ProcessBlockCollision(x, y);
			return CollisionResult::CollidedWithBrick;
		}
	}

	return CollisionResult::NoCollision;
}


CollisionResult CollisionManager::CollisionMoveUp(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY)
{
	int32_t x, y;
	y =  ((i_pos.y + 1) / m_tileSize) % m_mapSizeY;
	x = (i_pos.x + i_size.x/2) / m_tileSize;
	if (m_staticCollisions[m_currentMap][x][y] == "0")
	{
	return CollisionResult::NoCollision;
	}
	else
	{
		*i_posY = m_tileSize * (y+1);
		if (m_staticCollisions[m_currentMap][x][y] == "X"
			|| m_staticCollisions[m_currentMap][x][y] == "I")
		{
			return CollisionResult::CollidedWithStaticBlock;
		}
		else
		{
			ProcessBlockCollision(x, y);
			return CollisionResult::CollidedWithBrick;
		}
	}

}


CollisionResult CollisionManager::CheckCollision(const int& i_posX, const int& i_posY)
{
	if (m_staticCollisions[m_currentMap][i_posX][i_posY] != "0")
	{
		if (m_staticCollisions[m_currentMap][i_posX][i_posY] == "X")
		{
			return CollisionResult::CollidedWithStaticBlock;
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
		float currVel = i_speed*(float(nstep) / nsteps);
		glm::vec2 new_pos = glm::vec2(originalPos.x + i_dir.x*currVel, originalPos.y + i_dir.y*currVel);
		uint32_t x_mid = uint32_t((i_pos.x + (i_size / 2)) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		uint32_t y_mid = uint32_t((i_pos.y + (i_size / 2)) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		uint32_t x_right = uint32_t((new_pos.x + i_size) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		uint32_t x_left = uint32_t((new_pos.x) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		uint32_t y_up = uint32_t(new_pos.y / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
		uint32_t y_down = uint32_t((new_pos.y + i_size + 1) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();

		if (m_staticCollisions[m_currentMap][x_mid][y_up] != "0" || m_staticCollisions[m_currentMap][x_mid][y_down] != "0" ||
			m_staticCollisions[m_currentMap][x_left][y_mid] != "0" || m_staticCollisions[m_currentMap][x_right][y_mid] != "0")
		{

			if (m_staticCollisions[m_currentMap][x_mid][y_up] == "I" || m_staticCollisions[m_currentMap][x_mid][y_down] == "I")
			{

				if (m_staticCollisions[m_currentMap][x_mid][y_up] == "I")
				{
					m_cameraMoveUpFunction();
					m_currentMap += 1;
					i_pos = glm::ivec2(i_pos.x, i_pos.y - m_tileSize*3);
					return CollisionResult::CollidedWithScreen;
				}
				else
				{
					m_cameraMoveDownFunction();
					if (m_currentMap > 0)
					{
						m_currentMap -= 1;
						i_pos = glm::ivec2(i_pos.x, i_pos.y + m_tileSize * 3);
					}
					return CollisionResult::CollidedWithScreen;
				}
			}

			uint32_t x, y, dir;
			std::tie(x, y, dir) = CheckDirectionOfCollision(x_mid, y_mid, x_right, x_left, y_up, y_down);

			if (CheckCollision(x, y) == CollisionResult::CollidedWithStaticBlock)
			{
				i_dir[dir] = -i_dir[dir];
				return CollisionResult::CollidedWithStaticBlock;
			}
			else
			{
				i_dir[dir] = -i_dir[dir];
				ProcessBlockCollision(x, y);
				return CollisionResult::CollidedWithStaticBlock;
			}

		}


		if (i_pos.y + i_size < m_player->GetPosition().y && CollisionPlayer(new_pos, i_size, i_dir[1], i_speed))
		{
			glm::vec2 newDirNoChange = i_dir;
			newDirNoChange[1] = -newDirNoChange[1];
			
			glm::vec2 ballDownPos = glm::vec2(new_pos.x+i_size/2, new_pos.y);
			glm::vec2 playerPos = m_player->GetPosition();
			playerPos.x += m_player->GetSize().x / 2.f;
			glm::vec2 playerToBall = glm::vec2(ballDownPos.x - playerPos.x, ballDownPos.y - playerPos.y);
			playerToBall = glm::normalize(playerToBall);
			
			i_dir = glm::normalize(playerToBall*0.7f+newDirNoChange*0.3f);
			i_pos.y = m_player->GetPosition().y - i_size;
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

void CollisionManager::ProcessBlockCollision(uint32_t i_x, uint32_t i_y)
{
	uint32_t blockPos = std::stoi(m_staticCollisions[m_currentMap][i_x][i_y]);
	uint32_t brickResistance = m_breakableBlocks[m_currentMap].at(blockPos)->GetResistance() - 1;
	m_breakableBlocks[m_currentMap].at(blockPos)->SetResistance(brickResistance);
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


void CollisionManager::SetUpStaticCollisions(const std::string& i_staticCollisionMapPath, const std::vector<std::unordered_set<std::shared_ptr<Brick>>>& i_bricks, const std::vector<std::unordered_set<std::shared_ptr<Coin>>>& i_coins)
{
	std::ifstream fInput;
	fInput.open(i_staticCollisionMapPath);

	if (!fInput.is_open())
	{
		std::cerr << "Physics static collisions wrong path, path is : " << i_staticCollisionMapPath;
		return;
	}

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
					m_breakableBlocks[i].at(brickCounter)->SetPosition(glm::vec2((k-1)*m_tileSize, j*m_tileSize));
					std::string pos = std::to_string(brickCounter);
					brickCounter++;
					m_staticCollisions[i][k][j] = pos;
					m_staticCollisions[i][k-1][j] = pos;
				}
				else if(c == 'N' || c == 'M')
				{
					m_breakableBlocks[i].emplace(brickCounter, (*coinsIt));
					++coinsIt;
					m_breakableBlocks[i].at(brickCounter)->SetPosition(glm::vec2((k - 1)*m_tileSize, (j-1)*m_tileSize));
					std::string pos = std::to_string(brickCounter);
					brickCounter++;
					m_staticCollisions[i][k][j] = pos;
					m_staticCollisions[i][k - 1][j] = pos;
 					m_staticCollisions[i][k][j-1] = pos;
 					m_staticCollisions[i][k - 1][j-1] = pos;
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
