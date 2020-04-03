#pragma once

#include "window.h"
#include "menu.h"

#include "../rasterizer/camera.h"
#include "../rasterizer/input_handler.h"
#include "../rasterizer/shader_manager.h"

namespace app
{
	class Application final
	{
	public:
		Application();

		~Application();

		void start();

	private:
		Window window;
		Menu menu;
		rasterizer::Camera camera;
		rasterizer::ShaderManager shader_manager;
		rasterizer::InputHandler input_handler;
		int type = 0;
	};
}
