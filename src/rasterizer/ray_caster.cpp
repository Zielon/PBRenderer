#include "ray_caster.h"

#include <thread>
#include <iostream>
#include <fstream>

#include "input_handler.h"

void rasterizer::RayCaster::pick(const std::shared_ptr<Shader>& shader) const{

	auto x = InputHandler::last_x;
	auto y = InputHandler::last_y;

	pbr::Intersection intersection;
	pbr::Ray ray = camera->cast_ray(glm::vec2(x, y), glm::vec2(0));

	if (scene->intersect(ray, intersection))
	{
		const pbr::Triangle* triangle = (pbr::Triangle*)intersection.object;
		shader->setIVec4("triangle", glm::ivec4(triangle->ids, triangle->object->id));
	}
	else shader->setIVec4("triangle", glm::vec4(-1));

}

inline float clamp(const float& lo, const float& hi, const float& v){

	return std::max(lo, std::min(hi, v));
}

void rasterizer::RayCaster::ray_cast_frame(){

	if (saving) return;

	saving = true;

	std::thread work([this](){

		glm::vec3* framebuffer = new glm::vec3[camera->film_size.x * camera->film_size.y];
		glm::vec3* pix = framebuffer;

		for (uint32_t y = 0; y < camera->film_size.y; ++y)
		{
			for (uint32_t x = 0; x < camera->film_size.x; ++x)
			{
				pbr::Intersection intersection;
				pbr::Ray ray = camera->cast_ray(glm::vec2(x, y), glm::vec2(0));

				if (scene->intersect(ray, intersection))
				{
					const pbr::Triangle* triangle = (pbr::Triangle*)intersection.object;
					*pix = glm::abs(glm::vec4(triangle->n.xyz(), 1.f));
				}
				else
					*pix = glm::vec4(1.f);

				pix++;
			}
		}

		std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
		ofs << "P6\n" << camera->film_size.x << " " << camera->film_size.y << "\n255\n";
		for (uint32_t i = 0; i < camera->film_size.y * camera->film_size.x; ++i)
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
