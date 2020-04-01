#include "application.h"
#include "../geometry/model.h"

Application::Application():
	window(1200, 800),
	menu(glm::ivec2(0, 0), glm::ivec2(200, 300)),
	shader("default.vert", "default.frag"){

	load();
}

void Application::start() const{

	while (!glfwWindowShouldClose(window.get()))
	{
		glfwPollEvents();

		shader.use();
		menu.draw();

		glfwSwapBuffers(window.get());
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void Application::load() const{
	Model model;
	model.loadModel();
}
