#include "application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../geometry/model.h"
#include "../rasterizer/camera.h"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

app::Application::Application():
	window(SCR_WIDTH, SCR_HEIGHT),
	menu(glm::ivec2(0, 0), glm::ivec2(200, 300)),
	shader("default.vert", "default.frag"),
	camera(glm::vec3(0.0f, 0.0f, 6.0f)),
	input_handler(window.get(), &camera, SCR_WIDTH, SCR_HEIGHT){}

app::Application::~Application() = default;

void app::Application::start(){

	general::Model mesh_model;

	while (!glfwWindowShouldClose(window.get()))
	{
		rasterizer::InputHandler::process();

		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		menu.draw();

		shader.use();

		glm::mat4 projection =
			glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);

		mesh_model.draw(shader);

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	glfwTerminate();
}
