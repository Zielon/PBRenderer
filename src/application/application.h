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
		int shader_type = 0;
		int model_type = 0;
		bool wireframe = false;

		void attach_menu();
	};
}
