#include "application.h"

#include <thread>
#include "../integrators/whitted.h"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

app::Application::Application():
	menu(glm::ivec2(0, 0), glm::ivec2(200, 300)),
	window(SCR_WIDTH, SCR_HEIGHT),
	camera(std::make_shared<pbr::ProjectiveCamera>(glm::ivec2(SCR_WIDTH, SCR_HEIGHT))),
	scene(std::make_shared<pbr::Scene>(camera)),
	ray_caster(std::make_shared<rasterizer::RayCaster>(scene, camera)),
	model_loader(std::make_shared<pbr::Loader>(scene)),
	input_handler(window.get(), camera, SCR_WIDTH, SCR_HEIGHT){

	attach_menu();
}

app::Application::~Application() = default;

/**
 * Main application loop
 */
void app::Application::start(){

	scene->build();

	begin_frame = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window.get()))
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		auto shader = shader_manager.reload(shader_type);

		if (glfwGetKey(window.get(), GLFW_KEY_C) == GLFW_PRESS)
			ray_caster->ray_cast_frame();

		if (glfwGetKey(window.get(), GLFW_KEY_R) == GLFW_PRESS)
			render();

		if (!is_rendering)
		{
			rasterizer::InputHandler::process();
			camera->update_shader(shader);
		}

		ray_caster->pick(shader, is_picking);
		scene->draw(shader, is_wireframe);
		menu.draw();

		glfwSwapBuffers(window.get());
		glfwPollEvents();

		fps();
	}

	glfwTerminate();
}

void app::Application::render(){

	if (is_rendering) return;

	is_rendering = true;
	begin_rendering = std::chrono::steady_clock::now();

	std::thread work([this](){
		pbr::WhittedIntegrator whitted(scene);
		whitted.render(progress);
		is_rendering = false;
		progress = 0;
	});

	work.detach();
}

void app::Application::fps(){

	const auto end_frame = std::chrono::high_resolution_clock::now();
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end_frame - begin_frame).count();
	frames++;

	if (seconds >= 1.f)
	{
		fps_rate = float(frames);
		frames = 0;
		begin_frame = std::chrono::high_resolution_clock::now();
	}
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
		ImGui::Checkbox("Show wireframe", &is_wireframe);
		ImGui::Checkbox("Picking", &is_picking);

		long long millis = 0;

		if (is_rendering)
		{
			const auto end = std::chrono::steady_clock::now();
			millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_rendering).count();
		}

		ImGui::Text("Rendering: %llu [ms]", millis);
		ImGui::ProgressBar(float(progress));
		ImGui::Text("FPS             [%.1f]", fps_rate);
		ImGui::Text("Camera movement [%s]", is_rendering ? "OFF" : "ON");
		ImGui::Separator();
		ImGui::Text("\n Program usage: \n"
			" 1) Camera keys: \n   W - forward \n   D - right \n   A - left \n   S - back \n"
			" 2) Rendering: \n   R - [pbr] render \n   C - [normal] ray cast \n");
	});
}
