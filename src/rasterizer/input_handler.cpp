#include "input_handler.h"
#include "camera_handler.h"

GLFWwindow* rasterizer::Input_handler::window = nullptr;
float rasterizer::Input_handler::last_x = 0;
float rasterizer::Input_handler::last_y = 0;
float rasterizer::Input_handler::delta_time = 0;
float rasterizer::Input_handler::last_frame = 0;
bool rasterizer::Input_handler::first_mouse = false;
rasterizer::Camera_handler rasterizer::Input_handler::camera_handler;
std::shared_ptr<general::Camera> rasterizer::Camera_handler::camera;

rasterizer::Input_handler::Input_handler(
	GLFWwindow* window, std::shared_ptr<general::Camera> camera, int width, int height){

	Input_handler::window = window;

	camera_handler.set_camera_handler(camera);

	last_x = width / 2.f;
	last_y = height / 2.f;

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void rasterizer::Input_handler::mouse_callback(GLFWwindow* window, double xpos, double ypos){

	if (first_mouse)
	{
		last_x = xpos;
		last_y = ypos;
		first_mouse = false;
	}

	const float x_offset = xpos - last_x;
	const float y_offset = ypos - last_y;

	last_x = xpos;
	last_y = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		camera_handler.process_mouse_movement(x_offset, y_offset);
	}
}

void rasterizer::Input_handler::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

	camera_handler.process_mouse_scroll(yoffset);
}

void rasterizer::Input_handler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){ }

void rasterizer::Input_handler::process(){

	time_update();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_handler.process_keyboard(Movement::FORWARD, delta_time);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_handler.process_keyboard(Movement::BACKWARD, delta_time);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_handler.process_keyboard(Movement::LEFT, delta_time);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_handler.process_keyboard(Movement::RIGHT, delta_time);
}

void rasterizer::Input_handler::time_update(){

	const float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
}
