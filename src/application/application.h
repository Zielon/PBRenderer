#pragma once

#include "window.h"
#include "menu.h"
#include "../opengl/shader.h"

class Application
{
public:
	Application();

	void start() const;

	void load() const;

private:
	Window window;
	Menu menu;
	Shader shader;
};
