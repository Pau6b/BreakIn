#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

namespace game
{
namespace visuals
{
// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:

	TileMap(const std::string& i_levelFile, const glm::vec2& i_minCoords, visuals::ShaderProgram& i_program);
	~TileMap();

	void render() const;
	void free();
	
	uint32_t getTileSize() const { return m_tileSize; }
	void WipeDoorPositions(const std::pair<uint32_t,uint32_t>& i_positionsToWipe, uint32_t i_map, uint32_t m_totalMapNumber);

private:
	bool loadLevel(const std::string& i_levelFile);
	void prepareArrays(bool i_first);

	GLuint m_vao;
	GLuint m_vbo;
	GLint m_posLocation, m_texCoordLocation;
	glm::ivec2 m_position, m_mapSize, m_tilesheetSize;
	uint32_t m_tileSize, m_blockSize;
	visuals::Texture m_tilesheet;
	glm::vec2 m_tileTexSize;
	int* m_map;
	int32_t m_replaceTile;
	const glm::vec2& m_minCoords;
	visuals::ShaderProgram& m_program;
};
}
}