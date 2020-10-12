#pragma once
#include "Scene.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application

namespace game
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
	void mousePress(int i_button);
	void mouseRelease(int i_button);
	
	bool getKey(int i_key) const;
	bool getSpecialKey(int i_key) const;

private:
	bool m_bPlay;                       // Continue to play game?
	Scene m_scene;                      // Scene to render
	bool m_keys[256], m_specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time

};
}


