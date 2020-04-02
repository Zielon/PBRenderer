#include "application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../geometry/model.h"
#include "../core/gl_camera.h"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;
pbr::GLCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	const float xoffset = xpos - lastX;
	const float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(pbr::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(pbr::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(pbr::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(pbr::RIGHT, deltaTime);
}

pbr::Application::Application():
	window(SCR_WIDTH, SCR_HEIGHT),
	menu(glm::ivec2(0, 0), glm::ivec2(200, 300)),
	shader("default.vert", "default.frag"){}

void pbr::Application::start() const{

	Model mesh_model;

	while (!glfwWindowShouldClose(window.get()))
	{
		float current_frame = glfwGetTime();

		deltaTime = current_frame - lastFrame;
		lastFrame = current_frame;

		processInput(window.get());
		
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		menu.draw();

		shader.use();

		glm::mat4 projection =
			glm::perspective(glm::radians(camera.Zoom),
			                 static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
			                 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setMat4("model", model);

		mesh_model.draw(shader);

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	glfwTerminate();
}
