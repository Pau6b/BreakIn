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


CollisionResult CollisionManager::collisionMoveLeft(const glm::ivec2& i_pos, const glm::ivec2& i_size) const
{
	int x, y0, y1;

	x = i_pos.x / m_tileSize;
	y0 = i_pos.y / m_tileSize;
	y1 = (i_pos.y + i_size.y - 1) / m_tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] != '0')
			return CollisionResult::CollidedWithStaticBlock;
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::collisionMoveRight(const glm::ivec2& i_pos, const glm::ivec2& i_size) const
{
	int x, y0, y1;

	x = (i_pos.x + i_size.x - 1) / m_tileSize;
	y0 = i_pos.y / m_tileSize;
	y1 = (i_pos.y + i_size.y - 1) / m_tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (m_staticCollisions[m_currentMap][x][y] != '0')
			return CollisionResult::CollidedWithStaticBlock;
	}

	return CollisionResult::NoCollision;
}

CollisionResult CollisionManager::collisionMoveDown(const glm::ivec2& i_pos, const glm::ivec2& i_size, int* i_posY) const
{
	int x0, x1, y;

	x0 = i_pos.x / m_tileSize;
	x1 = (i_pos.x + i_size.x - 1) / m_tileSize;
	y = (i_pos.y + i_size.y - 1) / m_tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (m_staticCollisions[m_currentMap][y][x] != '0')
		{
			if (*i_posY - m_tileSize * y + i_size.y <= 4)
			{
				*i_posY = m_tileSize * y - i_size.y;
				return CollisionResult::CollidedWithStaticBlock;
			}
		}
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
		for (int j = sizey-1; j >= 0; --j)
		{
			for (int k = 0; k < sizex; ++k)
			{
				char c;
				fInput.get(c);
				m_staticCollisions[i][j][k] = c;
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