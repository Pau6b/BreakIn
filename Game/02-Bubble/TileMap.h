#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

namespace game
{
namespace gameplay
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
	
private:
	bool loadLevel(const std::string& i_levelFile);
	void prepareArrays(const glm::vec2& i_minCoords, visuals::ShaderProgram& i_program);

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLint m_posLocation, m_texCoordLocation;
	glm::ivec2 m_position, m_mapSize, m_tilesheetSize;
	uint32_t m_tileSize, m_blockSize;
	visuals::Texture m_tilesheet;
	glm::vec2 m_tileTexSize;
	int *m_map;

};
}
}