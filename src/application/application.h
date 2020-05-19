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

using namespace std::chrono;

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
		std::unique_ptr<rasterizer::RayCaster> ray_caster;
		std::shared_ptr<pbr::Loader> model_loader;
		rasterizer::ShaderManager shader_manager;
		rasterizer::InputHandler input_handler;

		// Menu options
		int shader_type{0};
		int integrator_type{0};
		float progress{0.f};
		time_point<steady_clock> begin_rendering;
		time_point<steady_clock> begin_frame;
		bool is_wireframe = false;
		bool is_picking = false;
		bool is_rendering = false;
		float fps_rate{0.f};
		int frames{0};
		int num_samples{10};
		int current_config{1};

		void attach_menu();

		void render();

		void fps();

		void reload();
	};
}
