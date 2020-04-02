#pragma once

#include "window.h"
#include "menu.h"
#include "../rasterizer/shader.h"
#include "../rasterizer/camera.h"
#include "../rasterizer/input_handler.h"

namespace app
{
	class Application
	{
	public:
		Application();

		~Application();

		void start();

	private:
		Window window;
		Menu menu;
		rasterizer::Shader shader;
		rasterizer::Camera camera;
		rasterizer::InputHandler input_handler;
	};
}
