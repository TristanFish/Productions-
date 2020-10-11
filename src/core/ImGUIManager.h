#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H
#include "graphics/Window.h"

class ImGUIManager
{

private:
	Window* window;

public:
	ImGUIManager(Window* inWindow);
	~ImGUIManager();

	void Run();
};

#endif