#include "application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../geometry/model.h"
#include "../rasterizer/camera.h"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;
const float ASPECT = float(SCR_WIDTH) / float(SCR_HEIGHT);

app::Application::Application():
	window(SCR_WIDTH, SCR_HEIGHT),
	menu(glm::ivec2(0, 0), glm::ivec2(200, 300)),
	shader("default.vert", "default.frag"),
	camera(glm::vec3(0.0f, 0.0f, 6.0f)),
	input_handler(window.get(), &camera, SCR_WIDTH, SCR_HEIGHT){

	// Some constant initial model transformation
	glm::mat4 model = glm::mat4(1.0f);
	model = translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

	shader.use();
	shader.setMat4("model", model);
}

app::Application::~Application() = default;

void app::Application::start(){

	general::Model model;

	while (!glfwWindowShouldClose(window.get()))
	{
		rasterizer::InputHandler::process();

		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		menu.draw();

		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ASPECT, 0.1f, 100.0f);

		shader.setMat4("projection", projection);
		shader.setMat4("view", camera.GetViewMatrix());

		model.draw(shader);

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	glfwTerminate();
}
