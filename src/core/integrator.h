#pragma once

#include "scene.h"
#include "film.h"
#include "uniform_sampler.h"
#include <thread>

namespace pbr
{
	class Integrator
	{
	public:
		Integrator(std::shared_ptr<Scene> scene): scene(std::move(scene)){

			for (auto i = 0; i < std::thread::hardware_concurrency(); ++i)
				samplers.push_back(std::make_shared<UniformSampler>());
		}

		virtual ~Integrator() = default;

		virtual void render();

	protected:
		virtual glm::vec3 Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const = 0;

		virtual std::shared_ptr<Film> get_film() const;

		virtual std::shared_ptr<Camera> get_camera() const;

		int num_samples = 4;
		std::shared_ptr<Scene> scene;
		std::vector<std::shared_ptr<Sampler>> samplers;
	};
}
