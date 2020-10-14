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