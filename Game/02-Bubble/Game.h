#pragma once
#include <memory>
#include "LevelScene.h"
#include "SceneManager.h"
#include "CheatSystem.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Game is a singleton (a class with a single instance) that represents our whole application

namespace game
{
namespace core
{
class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int i_key);
	void keyReleased(int i_key);
	void specialKeyPressed(int i_key);
	void specialKeyReleased(int i_key);
	void mouseMove(int i_x, int i_y);
	void mousePress(int i_button, int i_x, int i_y);
	void mouseRelease(int i_button, int i_x, int i_y);
	
	bool getKey(int i_key) const;
	bool getSpecialKey(int i_key) const;
	std::pair<int32_t, int32_t> getMousePos();

private:
	bool m_bPlay;                       // Continue to play game?
	std::unique_ptr<CheatSystem> m_cheatSystem;
	std::unique_ptr<SceneManager> m_sceneManager;
	bool m_keys[256], m_specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	std::pair<int32_t, int32_t> m_mousePos = { 0,0 };
};
}
}


