#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Sprite.h"
#include "Door.h"

namespace game
{
namespace visuals
{
TileMap::TileMap(const std::string& i_levelFile, const glm::vec2& i_minCoords, visuals::ShaderProgram& i_program)
	: m_minCoords(i_minCoords)
	, m_program(i_program)
{
	loadLevel(i_levelFile);
	prepareArrays(true);
}

TileMap::~TileMap()
{
	if(m_map != NULL)
		delete m_map;
}


void TileMap::Update(uint32_t i_delta)
{
	if (m_doorToUpdate != nullptr)
	{
		m_doorToUpdate->Update(i_delta);
		if (m_doorToUpdate->HasAnimationFinished())
		{
			m_doorToUpdate = nullptr;
		}
	}
}

void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	m_tilesheet.use();
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(m_posLocation);
	glEnableVertexAttribArray(m_texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * m_mapSize.x * m_mapSize.y);
	glDisable(GL_TEXTURE_2D);
	auto it = m_doorSprites.find(m_currentMap);
	if (it != m_doorSprites.end())
	{
		it->second->Render();
	}
	if (m_doorToUpdate != nullptr)
	{
		m_doorToUpdate->Render();
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &m_vbo);
}

void TileMap::WipeDoorPositions(uint32_t i_map)
{
	m_doorToUpdate = std::move(m_doorSprites.at(i_map));
	m_doorSprites.erase(i_map);
}

void TileMap::SetDoor(uint32_t i_map, std::unique_ptr<visuals::Door> i_door)
{
	m_doorSprites.emplace(i_map, std::move(i_door));
}

TileMap::BorderBlockInfo TileMap::GetBorderBlockInfo()
{
	return m_borderBlockInfo;
}

void TileMap::SetCurrentMap(uint32_t i_currentMap)
{
	m_currentMap = i_currentMap;
}

bool TileMap::HasDoorAnimationFinished()
{
	return !m_doorToUpdate;
}

void TileMap::EraseDoor(uint32_t i_map)
{
	auto it = m_doorSprites.find(i_map);
	if (it!= m_doorSprites.end())
	{
		m_doorSprites.erase(it);
	}
}

bool TileMap::loadLevel(const std::string& i_levelFile)
{
	std::ifstream fin;
	std::string line, tilesheetFile;
	std::stringstream sstream;

	fin.open(i_levelFile.c_str());
	if (!fin.is_open())
		return false;
	std::getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	std::getline(fin, line);
	sstream.str(line);
	sstream >> m_mapSize.x >> m_mapSize.y;
	std::getline(fin, line);
	sstream.str(line);
	sstream >> m_tileSize >> m_blockSize;
	std::getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	m_tilesheet.loadFromFile(tilesheetFile, visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGB);
	m_tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	m_tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	m_tilesheet.setMinFilter(GL_NEAREST);
	m_tilesheet.setMagFilter(GL_NEAREST);
	std::getline(fin, line);
	sstream.str(line);
	sstream >> m_tilesheetSize.x >> m_tilesheetSize.y;
	m_tileTexSize = glm::vec2(1.f / m_tilesheetSize.x, 1.f / m_tilesheetSize.y);
	std::getline(fin, line);
	sstream.str(line);
	sstream >> m_borderBlockTile;
	//It's horrible but i will work
	m_borderBlockInfo.texturePixelFormat = visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGB;
	if (m_borderBlockTile == 4)
	{
		m_borderBlockInfo.texturePath = "images/Border1.png";
	}
	else if (m_borderBlockTile == 8)
	{
		m_borderBlockInfo.texturePath = "images/Border2.png";
	}
	else if (m_borderBlockTile == 12)
	{
		m_borderBlockInfo.texturePath = "images/Border3.png";
	}
	m_map = new int[m_mapSize.x * m_mapSize.y];
	for (int j = 0; j < m_mapSize.y; j++)
	{
		for (int i = 0; i < m_mapSize.x; i++)
		{
			uint32_t tile;
			fin >> tile;
			m_map[j*m_mapSize.x + i] = tile;
		}
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(bool i_first)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	std::vector<float> vertices;

	halfTexel = glm::vec2(0.5f / m_tilesheet.width(), 0.5f / m_tilesheet.height());
	uint32_t offset = 5;
	for (int j = 0; j < m_mapSize.y; j++)
	{
		for (int i = 0; i < m_mapSize.x; i++)
		{
			tile = m_map[j * m_mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(m_minCoords.x + i * m_tileSize, m_minCoords.y + j * m_tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % m_tilesheetSize.x) / m_tilesheetSize.x, float((tile - 1) / m_tilesheetSize.x) / m_tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + m_tileTexSize;
				texCoordTile[0] += halfTexel;
				//texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + m_blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + m_blockSize); vertices.push_back(posTile.y + m_blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + m_blockSize); vertices.push_back(posTile.y + m_blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + m_blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}
	if (!i_first)
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	m_posLocation = m_program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	m_texCoordLocation = m_program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));

}

}
}