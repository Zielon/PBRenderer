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
	camera(glm::vec3(0.0f, 0.0f, 6.0f)),
	input_handler(window.get(), &camera, SCR_WIDTH, SCR_HEIGHT){

	const char* shaders[] = {"FLAT", "SHADER"};

	menu.attach([shaders, this](){
		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::CollapsingHeader("Current shader", ImGuiTreeNodeFlags_Selected))
		{
			ImGui::ListBox("", &type, shaders, IM_ARRAYSIZE(shaders), 2);
		}
	});
}

app::Application::~Application() = default;

void app::Application::start(){

	general::Model model;

	while (!glfwWindowShouldClose(window.get()))
	{
		rasterizer::InputHandler::process();

		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		auto shader = shader_manager.reload(type);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ASPECT, 0.1f, 100.0f);

		glm::mat4 mesh = glm::mat4(1.0f);
		mesh = translate(mesh, glm::vec3(0.0f, -1.75f, 0.0f));
		mesh = scale(mesh, glm::vec3(0.01f, 0.01f, 0.01f));

		shader->setMat4("model", mesh);
		shader->setMat4("projection", projection);
		shader->setMat4("view", camera.GetViewMatrix());
		shader->setVec3("camera_position", camera.Position);

		model.draw(shader);
		menu.draw();

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	glfwTerminate();
}
