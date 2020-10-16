#include "CollisionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace game
{
namespace physics
{
CollisionManager::CollisionManager(const std::string& i_staticCollisionsPath, const uint32_t i_tileSize)
	: m_tileSize(i_tileSize)
{
	SetUpStaticCollisions(i_staticCollisionsPath);
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.


CollisionResult CollisionManager::CollisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size) const
{
	int x, y0, y1;

	x = i_pos.x / m_tileSize;
	y0 =  i_pos.y / m_tileSize;
	y1 =  (i_pos.y + i_size.y - 1) / m_tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] != '0')
			return CollisionResult::CollidedWithStaticBlock;
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::CollisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size) const
{
	int x, y0, y1;

	x = (i_pos.x + i_size.x - 1) / m_tileSize;
	y0 =  i_pos.y / m_tileSize;
	y1 =  (i_pos.y + i_size.y - 1) / m_tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] != '0')
			return CollisionResult::CollidedWithStaticBlock;
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::CollisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY) const
{
	int x, y;
	y = (i_pos.y + i_size.y - 1) / m_tileSize;
	x =  (i_pos.x + i_size.x/2) / m_tileSize;
	if (m_staticCollisions[m_currentMap][x][y] != '0')
	{
		*i_posY = m_tileSize * y - i_size.y;
		return CollisionResult::CollidedWithStaticBlock;
	}

	return CollisionResult::NoCollision;
}


CollisionResult CollisionManager::CollisionMoveUp(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY) const
{
	int x, y;
	y =  (i_pos.y + 1) / m_tileSize;
	x = (i_pos.x + i_size.x) / m_tileSize;
	if (m_staticCollisions[m_currentMap][x][y] != '0')
	{
		*i_posY = m_tileSize * (y+1);
		return CollisionResult::CollidedWithStaticBlock;
	}

	return CollisionResult::NoCollision;
}


void CollisionManager::SetUpStaticCollisions(const std::string& i_staticCollisionMapPath)
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
	m_staticCollisions = std::vector<Matrix<char>>(levelQuantity, Matrix<char>(sizey, Row<char>(sizex)));
	for (int i = levelQuantity-1; i >= 0; --i)
	{
		for (int j = 0; j < sizey; ++j)
		{
			for (int k = 0; k < sizex; ++k)
			{
				char c;
				fInput.get(c);
				m_staticCollisions[i][k][j] = c;
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