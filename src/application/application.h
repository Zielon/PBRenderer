#pragma once

#include "window.h"
#include "menu.h"
#include "../opengl/shader.h"

namespace pbr
{
	class Application
	{
	public:
		Application();

		void start() const;

	private:
		Window window;
		Menu menu;
		Shader shader;
	};
}
