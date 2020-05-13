#pragma once

#include "../application/window.h"
#include "camera_handler.h"

namespace rasterizer
{
	class InputHandler
	{
	public:
		InputHandler(GLFWwindow* window, std::shared_ptr<pbr::Camera> camera, int width, int height);

		static void process();

		static void time_update();

		static float last_x;
		static float last_y;
	private:
		static GLFWwindow* window;
		static CameraHandler camera_handler;

		static bool first_mouse;
		static float delta_time;
		static float last_frame;

		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	};
}
