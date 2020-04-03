#include "input_handler.h"
#include "camera.h"

GLFWwindow* rasterizer::InputHandler::window = nullptr;
rasterizer::Camera* rasterizer::InputHandler::camera = nullptr;;
float rasterizer::InputHandler::last_x = 0;
float rasterizer::InputHandler::last_y = 0;
float rasterizer::InputHandler::delta_time = 0;
float rasterizer::InputHandler::last_frame = 0;
bool rasterizer::InputHandler::first_mouse = false;

rasterizer::InputHandler::InputHandler(GLFWwindow* window, Camera* camera, int width, int height){

	InputHandler::window = window;
	InputHandler::camera = camera;
	last_x = width / 2.f;
	last_y = height / 2.f;

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void rasterizer::InputHandler::mouse_callback(GLFWwindow* window, double xpos, double ypos){

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
		camera->ProcessMouseMovement(x_offset, y_offset);
	}
}

void rasterizer::InputHandler::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

	camera->ProcessMouseScroll(yoffset);
}

void rasterizer::InputHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){ }

void rasterizer::InputHandler::process(){

	time_update();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, delta_time);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, delta_time);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, delta_time);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, delta_time);
}

void rasterizer::InputHandler::time_update(){

	const float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
}
