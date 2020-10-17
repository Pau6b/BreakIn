#include "CollisionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace game
{
namespace physics
{
CollisionManager::CollisionManager(const std::string& i_staticCollisionsPath, const uint32_t i_tileSize, const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks)
	: m_tileSize(i_tileSize)
{
	SetUpStaticCollisions(i_staticCollisionsPath, i_bricks);
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.


CollisionResult CollisionManager::CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size)
{
	uint32_t x, y0, y1;

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
	uint32_t x, y0, y1;

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
	uint32_t x, y;
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
	uint32_t x, y;
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
	uint32_t brickResistance = m_bricks[m_currentMap].at(blockPos)->GetResistance() - 1;
	m_bricks[m_currentMap].at(blockPos)->SetResistance(brickResistance);
	if (brickResistance == 0)
	{
		m_bricks[m_currentMap].erase(blockPos);
		std::string blockId = m_staticCollisions[m_currentMap][i_x][i_y];
		m_staticCollisions[m_currentMap][i_x][i_y] = '0';
		if (m_staticCollisions[m_currentMap][i_x + 1][i_y] == blockId)
		{
			m_staticCollisions[m_currentMap][i_x+1][i_y] = '0';
		}
		else if(m_staticCollisions[m_currentMap][i_x - 1][i_y] == blockId)
		{
			m_staticCollisions[m_currentMap][i_x-1][i_y] = '0';
		}
	}
	
}


void CollisionManager::SetUpStaticCollisions(const std::string& i_staticCollisionMapPath, const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks)
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
	getline(fInput, line);
	sstream.str(line);
	sstream >> levelQuantity >> sizex >> sizey;
	m_staticCollisions = std::vector<Matrix<std::string>>(levelQuantity, Matrix<std::string>(sizey, Row<std::string>(sizex)));
	uint32_t brickCounter = 1;
	m_bricks = std::vector<std::map<uint32_t,std::shared_ptr<Brick>>>(levelQuantity);
	for (int i = levelQuantity-1; i >= 0; --i)
	{
		for (int j = 0; j < sizey; ++j)
		{
			for (int k = 0; k < sizex; ++k)
			{
				char c;
				fInput.get(c);
				if (c == '1' || c == '2' || c == '3')
				{
					m_bricks[i].emplace(brickCounter, i_bricks[i][brickCounter - 1]);
					m_bricks[i].at(brickCounter)->SetPosition(glm::vec2((k-1)*m_tileSize, j*m_tileSize));
					std::string pos = std::to_string(brickCounter);
					brickCounter++;
					m_staticCollisions[i][k][j] = pos;
					m_staticCollisions[i][k-1][j] = pos;
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