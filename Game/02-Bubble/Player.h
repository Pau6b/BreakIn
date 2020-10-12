#pragma once
#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


namespace game
{
class Player
{

public:
	void init(const glm::ivec2& i_tileMapPos, ShaderProgram& i_shaderProgram);
	void update(int i_deltaTime);
	void render();
	
	void setTileMap(TileMap* i_tileMap);
	void setPosition(const glm::vec2& i_pos);
	
private:
	bool m_bJumping;
	glm::ivec2 m_tileMapDispl, m_posPlayer;
	int m_jumpAngle, m_startY;
	Texture m_spritesheet;
	Sprite* m_sprite;
	TileMap* m_map;

};
}

