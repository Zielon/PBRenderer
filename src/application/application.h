#pragma once

#include "window.h"
#include "menu.h"

#include "../rasterizer/input_handler.h"
#include "../rasterizer/shader_manager.h"
#include "../cameras/projective.h"
#include "../core/scene.h"
#include "../geometry/loader.h"
#include "../rasterizer/ray_caster.h"
#include <atomic>

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
		std::shared_ptr<pbr::ProjectiveCamera> camera;
		std::shared_ptr<pbr::Scene> scene;
		std::shared_ptr<rasterizer::RayCaster> ray_caster;
		std::shared_ptr<pbr::Loader> model_loader;
		rasterizer::ShaderManager shader_manager;
		rasterizer::InputHandler input_handler;

		// Menu options
		int shader_type = 0;
		std::atomic<float> progress = 0.f;
		std::chrono::time_point<std::chrono::steady_clock> begin_rendering;
		bool is_wireframe = false;
		bool is_picking = false;
		bool is_rendering = false;

		void attach_menu();

		void render();
	};
}
