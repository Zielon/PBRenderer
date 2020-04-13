#pragma once

#include "window.h"
#include "menu.h"

#include "../rasterizer/input_handler.h"
#include "../rasterizer/shader_manager.h"
#include "../cameras/projective.h"
#include "../core/scene.h"
#include "../geometry/model_loader.h"
#include "../rasterizer/ray_caster.h"

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
		std::shared_ptr<pbr::projective_camera> camera;
		std::shared_ptr<pbr::Scene> scene;
		std::shared_ptr<rasterizer::RayCaster> ray_caster;
		std::shared_ptr<general::ModelLoader> model_loader;
		rasterizer::ShaderManager shader_manager;
		rasterizer::InputHandler input_handler;

		// Menu options
		int shader_type = 0;
		bool wireframe = false;

		void attach_menu();
	};
}
