#pragma once

#include "window.h"
#include "menu.h"

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
		Menu menu;
		Window window;
		std::shared_ptr<general::Camera> camera;
		rasterizer::Shader_manager shader_manager;
		rasterizer::Input_handler input_handler;
		int shader_type = 0;
		int model_type = 0;
		bool wireframe = false;

		void attach_menu();
	};
}
