#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <tchar.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//Remove console (only works in Visual Studio)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib, "irrKlang.lib")


#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps

static int prevTime;
static game::core::Game gameInstance; // This object represents our whole game


// If a key is pressed this callback is called

static void keyboardDownCallback(unsigned char key, int x, int y)
{
	game::core::Game::instance().keyPressed(key);
}

// If a key is released this callback is called

static void keyboardUpCallback(unsigned char key, int x, int y)
{
	game::core::Game::instance().keyReleased(key);
}

// If a special key is pressed this callback is called

static void specialDownCallback(int key, int x, int y)
{
	game::core::Game::instance().specialKeyPressed(key);
}

// If a special key is released this callback is called

static void specialUpCallback(int key, int x, int y)
{
	game::core::Game::instance().specialKeyReleased(key);
}

// Same for changes in mouse cursor position

static void motionCallback(int x, int y)
{
	game::core::Game::instance().mouseMove(x, y);
}

// Same for mouse button presses or releases

static void mouseCallback(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
		game::core::Game::instance().mousePress(button, x, y);
	else if(state == GLUT_UP)
		game::core::Game::instance().mouseRelease(button, x, y);
}

static void drawCallback()
{
	game::core::Game::instance().render();
	glutSwapBuffers();
}

static void resizeCallback(int width, int height)
{
	glutReshapeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
}

static void idleCallback()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - prevTime;
	
	if(deltaTime > TIME_PER_FRAME)
	{
		// Every time we enter here is equivalent to a game loop execution
		if(!game::core::Game::instance().update(deltaTime))
			exit(0);
		prevTime = currentTime;
		glutPostRedisplay();
	}
}


int main(int argc, char **argv)
{
	
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glutCreateWindow(argv[0]);
	glutDisplayFunc(drawCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardDownCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialDownCallback);
	glutSpecialUpFunc(specialUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	glutPassiveMotionFunc(motionCallback);
	glutReshapeFunc(resizeCallback);


	// GLEW will take care of OpenGL extension functions
	glewExperimental = GL_TRUE;
	glewInit();
	
	// Game instance initialization
	game::core::Game::instance().init();
	prevTime = glutGet(GLUT_ELAPSED_TIME);
	// GLUT gains control of the application
	glutMainLoop();

	return 0;
}


