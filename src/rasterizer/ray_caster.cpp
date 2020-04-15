#include "ray_caster.h"

#include <thread>
#include <iostream>
#include <fstream>

#include "input_handler.h"
#include "../core/uniform_sampler.h"
#include "../core/film.h"

void rasterizer::RayCaster::pick(const std::shared_ptr<Shader>& shader, bool picking) const{

	auto x = InputHandler::last_x;
	auto y = InputHandler::last_y;

	pbr::Intersection intersection;
	pbr::Ray ray = camera->cast_ray(glm::vec2(x, y), glm::vec2(0));

	if (scene->intersect(ray, intersection) && picking)
	{
		const pbr::Triangle* triangle = (pbr::Triangle*)intersection.triangle;
		shader->setIVec4("triangle", glm::ivec4(triangle->ids, triangle->scene_object->id));
	}
	else shader->setIVec4("triangle", glm::vec4(-1));

}

void rasterizer::RayCaster::ray_cast_frame(){

	if (saving) return;

	saving = true;

	std::thread work([this](){

		pbr::Film film(camera->film_size);

		for (int i = 0; i < 8; i++)
		{
			pbr::UniformSampler sampler(0.f, 1.f);

			const auto weight = 1.0f / (i + 1);

			for (uint32_t y = 0; y < camera->film_size.y; ++y)
			{
				for (uint32_t x = 0; x < camera->film_size.x; ++x)
				{
					pbr::Intersection intersection;
					pbr::Ray ray = camera->cast_ray(glm::vec2(x, y), sampler.get2D());

					if (scene->intersect(ray, intersection))
					{
						const pbr::Triangle* triangle = (pbr::Triangle*)intersection.triangle;

						auto pixel = film.get_pixel(x, y).to_vec3();
						pixel *= i * weight;
						pixel += weight * glm::abs(triangle->n);

						film.set_pixel(pixel, x, y);
					}
					else
						film.set_pixel(glm::vec3(1.f), x, y);
				}
			}
		}

		film.save_jpg("ray_cast.jpg");
		//film.save_ppm("ray_cast.ppm");
		std::cout << "FILM::SAVED" << std::endl;
		saving = false;
	});

	work.detach();
}
