#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10

namespace game
{
void Scene::init()
{
	initShaders();
	m_map = std::make_unique<TileMap>("levels/level1/visualTilemap.txt", glm::vec2(SCREEN_X, SCREEN_Y), m_texProgram);
	m_collisionManager = std::make_unique<physics::CollisionManager>("levels/level1/physics.txt", m_map->getTileSize());
	m_player = std::make_unique<Player>(*m_collisionManager);
	m_player->init(glm::ivec2(SCREEN_X, SCREEN_Y), m_texProgram);
	m_player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * m_map->getTileSize(), INIT_PLAYER_Y_TILES * m_map->getTileSize()));
	m_projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	m_currentTime = 0.0f;
}

void Scene::update(int i_deltaTime)
{
	m_currentTime += i_deltaTime;
	m_player->update(i_deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	m_texProgram.use();
	m_texProgram.setUniformMatrix4f("projection", m_projection);
	m_texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	m_texProgram.setUniformMatrix4f("modelview", modelview);
	m_texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	m_map->render();
	m_player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << "\n";
		std::cout << "" << vShader.log() << "\n\n";
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << "\n";
		std::cout << "" << fShader.log() << "\n\n";
	}
	m_texProgram.init();
	m_texProgram.addShader(vShader);
	m_texProgram.addShader(fShader);
	m_texProgram.link();
	if(!m_texProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << "\n";
		std::cout << "" << m_texProgram.log() << "\n\n";
	}
	m_texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
}

