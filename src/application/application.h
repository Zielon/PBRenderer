#pragma once

#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>

#include "window.h"
#include "menu.h"

#include "../rasterizer/input_handler.h"
#include "../rasterizer/shader_manager.h"
#include "../cameras/projective.h"

namespace general
{
	class Mesh;
}

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
		rasterizer::Shader_manager shader_manager;
		rasterizer::Input_handler input_handler;

		bool saving = false;

		// Menu options
		int shader_type = 0;
		int model_type = 0;
		bool wireframe = false;

		void attach_menu();

		void ray_cast_frame(const general::Mesh& mesh);

		void pick(const general::Mesh& mesh, const std::shared_ptr<rasterizer::Shader>& shader) const;
	};
}
