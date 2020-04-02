#pragma once
#include "../application/window.h"
#include "camera.h"

namespace rasterizer
{
	class InputHandler
	{
	public:

		static GLFWwindow* window;
		static Camera* camera;
		static float last_x;
		static float last_y;
		static bool first_mouse;
		static float delta_time;
		static float last_frame;

		InputHandler(GLFWwindow* window, Camera* camera, int width, int height);

		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		static void process();

		static void time_update();
	};
}
