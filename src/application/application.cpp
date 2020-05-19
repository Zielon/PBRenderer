#include "application.h"

#include <windows.h>
#include <thread>
#include <filesystem>

#include "../rasterizer/input_handler.h"
#include "../integrators/whitted.h"
#include "../integrators/path_tracer.h"
#include "../integrators/direct_lighting.h"

const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 700;
const std::string DEFAULT = "../configuration/ajax_gold.json";

app::Application::Application():
	menu(glm::ivec2(0, 0), glm::ivec2(200, 300)),
	window(SCR_WIDTH, SCR_HEIGHT),
	camera(std::make_shared<pbr::ProjectiveCamera>(glm::ivec2(SCR_WIDTH, SCR_HEIGHT))),
	scene(std::make_shared<pbr::Scene>(camera)),
	ray_caster(std::make_unique<rasterizer::RayCaster>(scene, camera)),
	model_loader(std::make_shared<pbr::Loader>(scene, DEFAULT)),
	input_handler(window.get(), camera, SCR_WIDTH, SCR_HEIGHT){

	model_loader->load_camera(DEFAULT, camera.get());
	attach_menu();
}

app::Application::~Application() = default;

/**
 * Main application loop
 */
void app::Application::start(){

	begin_frame = high_resolution_clock::now();

	using rasterizer::InputHandler;

	while (!glfwWindowShouldClose(window.get()))
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		auto shader = shader_manager.reload(shader_type);

		camera->is_active = !is_rendering && !ray_caster->is_saving && !ImGui::GetIO().WantCaptureMouse;

		if (glfwGetKey(window.get(), GLFW_KEY_C) == GLFW_PRESS)
			ray_caster->ray_cast_frame();

		if (glfwGetKey(window.get(), GLFW_KEY_R) == GLFW_PRESS)
			render();

		if (camera->is_active)
		{
			InputHandler::process();
			camera->update_shader(shader);
			ray_caster->pick(shader, is_picking);
		}

		scene->draw(shader, is_wireframe);
		menu.draw();

		glfwSwapBuffers(window.get());
		glfwPollEvents();
		fps();
		reload();
		InputHandler::time_update();
	}

	glfwTerminate();
	exit(0);
}

void app::Application::render(){

	if (is_rendering) return;

	is_rendering = true;
	begin_rendering = steady_clock::now();

	std::shared_ptr<pbr::Integrator> integrator{};

	switch (integrator_type)
	{
	case 0:
		integrator = std::make_shared<pbr::PathTracer>(scene, num_samples);
		break;
	case 1:
		integrator = std::make_shared<pbr::WhittedIntegrator>(scene, num_samples);
		break;
	case 2:
		integrator = std::make_shared<pbr::DirectLighting>(scene, num_samples);
		break;
	default:
		break;
	}

	std::thread work([this, integrator](){
		//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		integrator->render(progress);
		is_rendering = false;
		progress = 0;
	});

	work.detach();
}

void app::Application::fps(){

	const auto end_frame = high_resolution_clock::now();
	const auto seconds = duration_cast<std::chrono::seconds>(end_frame - begin_frame).count();
	frames++;

	if (seconds >= 1.f)
	{
		fps_rate = float(frames);
		frames = 0;
		begin_frame = high_resolution_clock::now();
	}
}

void app::Application::reload(){

	std::string path;

	if (current_config == 0) path = "../configuration/ajax_glass.json";
	if (current_config == 1) path = "../configuration/ajax_gold.json";
	if (current_config == 2) path = "../configuration/ajax_plastic.json";
	if (current_config == 3) path = "../configuration/cornell_box.json";

	if (model_loader->needs_reload(path))
	{
		scene.reset(new pbr::Scene(camera));
		ray_caster.reset(new rasterizer::RayCaster(scene, camera));
		model_loader->reload(path, scene);
	}
}

void app::Application::attach_menu(){

	const char* shaders[] = {"FLAT", "NORMALS", "SMOOTH"};
	const char* integrators[] = {"PATH TRACER", "WHITTED", "DIRECT ILLUMINATION"};
	const char* configs[] = {"AJAX GLASS", "AJAX GOLD", "AJAX PLASTIC", "CORNELL BOX"};

	menu.attach([shaders, integrators, configs, this](){

		ImGui::Text("PBRenderer");

		ImGui::Separator();

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::CollapsingHeader("Current shader"))
			ImGui::ListBox("shaders", &shader_type, shaders, IM_ARRAYSIZE(shaders), 3);

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		ImGui::Checkbox("Show wireframe", &is_wireframe);
		ImGui::Checkbox("Picking", &is_picking);

		long long millis = 0;

		if (is_rendering)
		{
			const auto end = steady_clock::now();
			millis = duration_cast<milliseconds>(end - begin_rendering).count();
		}

		const auto position = camera->get_coordinate().get().position;
		const auto direction = camera->get_coordinate().get().direction;

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::CollapsingHeader("Current integrator"), ImGuiTreeNodeFlags_DefaultOpen)
			ImGui::ListBox("integrators", &integrator_type, integrators, IM_ARRAYSIZE(integrators), 3);

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		ImGui::Text("# samples");
		ImGui::SameLine();
		ImGui::InputInt("int", &num_samples, 1);
		ImGui::Text("PBR rendering: %4llu [ms]", millis);
		ImGui::ProgressBar(progress);
		ImGui::Text("FPS             [%3.1f]", fps_rate);
		ImGui::Text("Camera movement [%4s]", is_rendering || ray_caster->is_saving ? "OFF" : "ON");
		ImGui::Text("# threads       [%4i]", std::thread::hardware_concurrency());
		ImGui::Text("Camera position/direction");
		ImGui::Text("%.5f %.5f %.5f", position.x, position.y, position.z);
		ImGui::Text("%.5f %.5f %.5f", direction.x, direction.y, direction.z);
		ImGui::NewLine();
		ImGui::Text("%s", configs[current_config]);
		if (ImGui::CollapsingHeader("Configuration"))
			ImGui::ListBox("configs", &current_config, configs, IM_ARRAYSIZE(configs), 4);
		ImGui::Separator();
		ImGui::NewLine();
		ImGui::Text("Program usage: \n"
			" 1) Camera keys: \n   W - forward \n   D - right \n   A - left \n   S - back \n"
			" 2) Mouse: \n   use scroll \n   press left button [move] \n"
			" 3) Rendering keys: \n   R - [pbr] render \n   C - [normal] ray cast \n");
	});
}
