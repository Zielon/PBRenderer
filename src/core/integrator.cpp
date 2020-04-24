#include "integrator.h"

#include <omp.h>
#include <thread>

#include "uniform_sampler.h"
#include "../cameras/projective.h"

void pbr::Integrator::render(std::atomic<float>& progress){

	const auto start = std::chrono::steady_clock::now();

	const auto film_size = get_camera()->get_film()->get_size();
	const int width = int(film_size.x);
	const int height = int(film_size.y);

	std::vector<PixelSamples> pixels(height * width, PixelSamples());

	const auto work = float(num_samples * height * width);
	std::atomic<int> current{1};

	for (auto i = 0; i < num_samples; i++)
	{
		#pragma omp parallel num_threads(std::thread::hardware_concurrency())
		{
			#pragma omp for schedule(dynamic, 128)
			for (auto j = 0; j < height * width; ++j)
			{
				const auto y = int(j / width);
				const auto x = j - y * width;

				auto& sampler = samplers[omp_get_thread_num()];
				const auto offset = sampler->get2D();

				auto ray = get_camera()->cast_ray(glm::vec2(x, y), offset);
				pixels[j].emplace_back(Li(ray, sampler, 0), offset);

				progress = float(current) / work;
				++current;
			}
		}
	}

	get_film()->merge(pixels);
	get_film()->save_jpg("output.jpg");
	//get_film()->save_ppm("output.ppm");

	const auto end = std::chrono::steady_clock::now();
	const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "INFO::INTEGRATOR Render time: [" << millis << " ms]" << std::endl;
}

std::shared_ptr<pbr::Film> pbr::Integrator::get_film() const{

	return scene->get_camera()->get_film();
}

std::shared_ptr<pbr::Camera> pbr::Integrator::get_camera() const{

	return scene->get_camera();
}
