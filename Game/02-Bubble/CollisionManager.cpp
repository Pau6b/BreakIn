#include "CollisionManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Player.h"

namespace game
{
namespace gameplay
{
namespace physics
{
CollisionManager::CollisionManager(const std::string& i_staticCollisionsPath, const uint32_t i_tileSize, const std::vector<std::vector<std::shared_ptr<Brick>>>& i_bricks, std::function<void()> i_moveDown, std::function<void()> i_moveUp)
	: m_tileSize(i_tileSize)
	, m_cameraMoveDownFunction(i_moveDown)
	, m_cameraMoveUpFunction(i_moveUp)
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
	uint32_t x, y0, y1;

	x = (i_pos.x + i_size.x - 1) / m_tileSize;
	y0 =  i_pos.y / m_tileSize;
	y1 =  (i_pos.y + i_size.y - 1) / m_tileSize;
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
	uint32_t x, y;
	y = (i_pos.y + i_size.y - 1) / m_tileSize;
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

bool CollisionManager::CollisionPlayer(int i_Ydown, int i_Xmid, int i_Xleft, int i_Xright, int i_dirY, const int& i_Speed)
{
	glm::ivec2 posPlayer = m_player->getPosition();
	glm::ivec2 sizePlayer = m_player->getSize();

	uint32_t y_ini, y_end;
	y_ini = posPlayer.y;
	y_end = posPlayer.y + sizePlayer.y + i_Speed;
	return (i_Ydown >= y_ini && i_Ydown <= y_end && (i_Xmid >= posPlayer.x ) && i_Xmid <= (posPlayer.x + sizePlayer.x) && i_dirY > 0);

}

CollisionResult CollisionManager::CollisionBall(glm::ivec2& i_pos, glm::vec2& i_dir, const int& i_size, const int& i_speed)
{
	uint32_t x_right, x_left, y_up, y_down, x_mid, y_mid;
	glm::ivec2 new_pos = glm::ivec2(i_pos.x + i_dir.x*i_speed, i_pos.y + i_dir.y*i_speed);
	x_mid = ((i_pos.x + (i_size / 2)) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
	y_mid = ((i_pos.y + (i_size / 2)) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
	x_right = ((new_pos.x + i_size) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
	x_left = ((new_pos.x) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
	y_up = (new_pos.y / m_tileSize) % m_staticCollisions[m_currentMap][0].size();
	y_down = ((new_pos.y + i_size + 1) / m_tileSize) % m_staticCollisions[m_currentMap][0].size();


	
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
				m_currentMap -= 1;
				i_pos = glm::ivec2(i_pos.x, i_pos.y + m_tileSize * 3);
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


	if (CollisionPlayer(y_down*m_tileSize, x_mid*m_tileSize, x_left*m_tileSize, x_right*m_tileSize, i_dir[1], i_speed))
	{
		i_dir[1] = -i_dir[1];
		return CollisionResult::CollidedWithPlayer;
	}

	i_pos = new_pos;

	return CollisionResult::NoCollision;

}

void CollisionManager::LinkPlayer(Player* i_player)
{
	m_player = i_player;
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
	std::getline(fInput, line);
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
}