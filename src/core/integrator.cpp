#include "integrator.h"

#include <omp.h>
#include <thread>

#include "uniform_sampler.h"
#include "../cameras/projective.h"

void pbr::Integrator::render(){

	const auto start = std::chrono::steady_clock::now();

	const auto film_size = get_camera()->get_film()->get_size();
	const int width = int(film_size.x);
	const int height = int(film_size.y);

	for (auto i = 0; i < num_samples; i++)
	{
		const auto weight = 1.0f / (i + 1);

		#pragma omp parallel num_threads(std::thread::hardware_concurrency())
		{
			#pragma omp for schedule(dynamic, 8)
			for (int j = 0; j < height * width; ++j)
			{
				const auto y = int(j / width);
				const auto x = j - y * width;
				auto pixel = get_film()->get_pixel(x, y).to_vec3();

				auto& sampler = samplers[omp_get_thread_num()];
				auto ray = get_camera()->cast_ray(glm::vec2(x, y), sampler->get2D());

				pixel *= i * weight;
				pixel += weight * Li(ray, sampler, 0);
				get_film()->set_pixel(pixel, x, y);
			}
		}
	}

	get_film()->save_ppm("output.ppm");

	const auto end = std::chrono::steady_clock::now();

	std::cout << "INFO::RENDER Time: [" <<
		std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() <<
		" ms]" << std::endl;
}

std::shared_ptr<pbr::Film> pbr::Integrator::get_film() const{

	return scene->get_camera()->get_film();
}

std::shared_ptr<pbr::Camera> pbr::Integrator::get_camera() const{

	return scene->get_camera();
}
