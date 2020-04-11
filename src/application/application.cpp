#include "application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include "../geometry/model.h"
#include <thread>

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

app::Application::Application():
	menu(glm::ivec2(0, 0), glm::ivec2(200, 300)),
	window(SCR_WIDTH, SCR_HEIGHT),
	camera(std::make_shared<pbr::ProjectiveCamera>(glm::ivec2(SCR_WIDTH, SCR_HEIGHT))),
	input_handler(window.get(), camera, SCR_WIDTH, SCR_HEIGHT){

	attach_menu();
}

app::Application::~Application() = default;

inline float clamp(const float& lo, const float& hi, const float& v){
	return std::max(lo, std::min(hi, v));
}

/**
 * Main application loop
 */
void app::Application::start(){

	general::Model model;

	while (!glfwWindowShouldClose(window.get()))
	{
		rasterizer::Input_handler::process();

		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		auto shader = shader_manager.reload(shader_type);

		pick(model.meshes[0], shader);

		if (glfwGetKey(window.get(), GLFW_KEY_R) == GLFW_PRESS)
			ray_cast_frame(model.meshes[0]);

		shader->setMat4("projection", camera->camera_to_screen);
		shader->setMat4("view", camera->world_to_camera());
		shader->setVec3("direction", camera->direction);

		model.reload_model(model_type);
		model.draw(shader, wireframe);

		menu.draw();

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	glfwTerminate();
}

void app::Application::pick(const general::Mesh& mesh, const std::shared_ptr<rasterizer::Shader>& shader) const{

	auto x = rasterizer::Input_handler::last_x;
	auto y = rasterizer::Input_handler::last_y;
	pbr::Intersection intersection;
	mesh.intersect(camera->cast_ray(glm::vec2(x, y), glm::vec2(0)), intersection);
	const pbr::Triangle* triangle = (pbr::Triangle*)intersection.object;

	if (triangle)
		shader->setIVec3("triangle", triangle->ids);
	else
		shader->setIVec3("triangle", glm::vec3(-1, -1, -1));
}

void app::Application::attach_menu(){

	const char* shaders[] = {"FLAT", "TEXTURE"};
	const char* models[] = {"ARMADILLO", "NANOSUIT", "CYBORG"};

	menu.attach([shaders, models, this](){

		ImGui::Text("PBRenderer");

		ImGui::Separator();

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::CollapsingHeader("Current shader"))
		{
			ImGui::ListBox("shaders", &shader_type, shaders, IM_ARRAYSIZE(shaders), 2);
		}

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		if (ImGui::CollapsingHeader("Current model"))
		{
			ImGui::ListBox("models", &model_type, models, IM_ARRAYSIZE(models), 3);
		}

		ImGui::PushItemWidth(ImGui::GetWindowWidth());
		ImGui::Checkbox("Show wireframe", &wireframe);
	});
}

void app::Application::ray_cast_frame(const general::Mesh& mesh){

	if (saving) return;

	saving = true;

	std::thread work([&mesh, this](){

		glm::vec3* framebuffer = new glm::vec3[SCR_WIDTH * SCR_HEIGHT];
		glm::vec3* pix = framebuffer;

		for (uint32_t j = 0; j < SCR_HEIGHT; ++j)
		{
			for (uint32_t i = 0; i < SCR_WIDTH; ++i)
			{
				pbr::Intersection intersection;

				if (mesh.intersect(camera->cast_ray(glm::vec2(i, j), glm::vec2(0)), intersection))
				{
					const pbr::Triangle* triangle = (pbr::Triangle*)intersection.object;
					*pix = normalize(glm::mat3(camera->world_to_camera()) * glm::mat3(mesh.toWorld) * triangle->n);
				}

				pix++;
			}
		}

		std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
		ofs << "P6\n" << SCR_WIDTH << " " << SCR_HEIGHT << "\n255\n";
		for (uint32_t i = 0; i < SCR_HEIGHT * SCR_WIDTH; ++i)
		{
			char r = (char)(255 * clamp(0, 1, framebuffer[i].x));
			char g = (char)(255 * clamp(0, 1, framebuffer[i].y));
			char b = (char)(255 * clamp(0, 1, framebuffer[i].z));
			ofs << r << g << b;
		}

		ofs.close();
		delete[] framebuffer;
		std::cout << "FILM::SAVED" << std::endl;
		saving = false;
	});

	work.detach();
}
