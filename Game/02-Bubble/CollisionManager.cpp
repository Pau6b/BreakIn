#include "CollisionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "BreakableBlock.h"
#include "Brick.h"
#include "Coin.h"

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
								   std::function<void(std::shared_ptr<BreakableBlock> i_brokenBlock)> i_onBrokenBlockFunction)
	: m_tileSize(i_tileSize)
	, m_onBreakableBlockBroken(i_onBrokenBlockFunction)
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
	y0 =  i_pos.y / m_tileSize;
	y1 =  (i_pos.y + i_size.y - 1) / m_tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] != "0")
		{
			if (m_staticCollisions[m_currentMap][x][y] == "X")
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
	y0 =  i_pos.y / m_tileSize;
	y1 =  (i_pos.y + i_size.y - 1) / m_tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] != "0")
		{
			if (m_staticCollisions[m_currentMap][x][y] == "X")
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
	y = (i_pos.y + i_size.y - 1) / m_tileSize;
	x =  (i_pos.x + i_size.x/2) / m_tileSize;
	if (m_staticCollisions[m_currentMap][x][y] == "0")
	{
		return CollisionResult::NoCollision;
	}
	else
	{
		*i_posY = m_tileSize * y - i_size.y;
		if (y >= m_staticCollisions[m_currentMap][x].size())
		{
			return CollisionResult::CollidedWithDownScreen;
		}
		if (m_staticCollisions[m_currentMap][x][y] == "X")
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
	y =  (i_pos.y + 1) / m_tileSize;
	x = (i_pos.x + i_size.x/2) / m_tileSize;
	if (m_staticCollisions[m_currentMap][x][y] == "0")
	{
	return CollisionResult::NoCollision;
	}
	else
	{
		*i_posY = m_tileSize * (y+1);
		if (y >= m_staticCollisions[m_currentMap][x].size())
		{
			return CollisionResult::CollidedWithDownScreen;
		}
		if (m_staticCollisions[m_currentMap][x][y] == "X")
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
	int sizey;

	std::stringstream sstream;
	std::string line;
	std::getline(fInput, line);
	sstream.str(line);
	sstream >> levelQuantity >> sizex >> sizey;
	m_staticCollisions = std::vector<Matrix<std::string>>(levelQuantity, Matrix<std::string>(sizey, Row<std::string>(sizex)));
	uint32_t brickCounter = 1;
	m_breakableBlocks = std::vector<std::map<uint32_t,std::shared_ptr<BreakableBlock>>>(levelQuantity);
	for (int i = levelQuantity-1; i >= 0; --i)
	{
		auto blocksIt = i_bricks[i].begin();
		auto coinsIt = i_coins[i].begin();
		for (int j = 0; j < sizey; ++j)
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
					m_breakableBlocks[i].at(brickCounter)->SetPosition(glm::vec2((k - 1)*m_tileSize, j*m_tileSize));
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