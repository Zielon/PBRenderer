#include "application.h"

#include <thread>

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

app::Application::Application():
	menu(glm::ivec2(0, 0), glm::ivec2(200, 300)),
	window(SCR_WIDTH, SCR_HEIGHT),
	camera(std::make_shared<pbr::ProjectiveCamera>(glm::ivec2(SCR_WIDTH, SCR_HEIGHT))),
	scene(std::make_shared<pbr::Scene>(camera)),
	ray_caster(std::make_shared<rasterizer::RayCaster>(scene, camera)),
	model_loader(std::make_shared<general::ModelLoader>(scene)),
	input_handler(window.get(), camera, SCR_WIDTH, SCR_HEIGHT){

	attach_menu();
}

app::Application::~Application() = default;

/**
 * Main application loop
 */
void app::Application::start(){

	scene->build();

	while (!glfwWindowShouldClose(window.get()))
	{
		rasterizer::InputHandler::process();

		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		auto shader = shader_manager.reload(shader_type);

		if (glfwGetKey(window.get(), GLFW_KEY_R) == GLFW_PRESS)
			ray_caster->ray_cast_frame();

		ray_caster->pick(shader, picking);
		camera->update_shader(shader);
		scene->draw(shader, wireframe);

		menu.draw();

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	glfwTerminate();
}

void app::Application::attach_menu(){

	const char* shaders[] = {"FLAT", "NORMALS", "SMOOTH"};

	menu.attach([shaders, this](){

		ImGui::Text("PBRenderer");

		ImGui::Separator();

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::CollapsingHeader("Current shader"))
		{
			ImGui::ListBox("shaders", &shader_type, shaders, IM_ARRAYSIZE(shaders), 3);
		}

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		ImGui::Checkbox("Show wireframe", &wireframe);
		ImGui::Checkbox("Picking", &picking);
	});
}
