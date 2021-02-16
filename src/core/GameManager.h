#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "graphics/Window.h"
#include "Timer.h"
#include "sdl/SDL.h"
#include "scenes/Scene.h"
#include <Windows.h>
#include "scenes/Scene1.h"
class GameManager
{

private:
	Window* window;
	static Scene* currentScene;
	Timer timer;
	unsigned int fps;
	bool isRunning;
public:

	GameManager();
	~GameManager();

	static Scene* GetCurrentScene () { return currentScene; }

	void Init();
	void Run();
	void HandleEvents();

	void ReloadCurrentScene();

};


#endif