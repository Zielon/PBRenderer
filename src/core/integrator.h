#pragma once

#include "scene.h"
#include "film.h"
#include "uniform_sampler.h"

#include <thread>
#include <glm/glm.hpp>
#include <atomic>

namespace pbr
{
	class Integrator
	{
	public:
		Integrator(std::shared_ptr<Scene> scene, int num_samples): num_samples(num_samples), scene(std::move(scene)){

			for (auto i = 0; i < std::thread::hardware_concurrency(); ++i)
				samplers.push_back(std::make_shared<UniformSampler>());
		}

		virtual ~Integrator() = default;

		virtual void render(std::atomic<float>& progress);

	protected:
		virtual glm::vec3 Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const = 0;

		virtual glm::vec3 reflect(
			const Ray& ray, const std::shared_ptr<Sampler>& sampler, Intersection& isect, int depth) const;

		virtual glm::vec3 transmit(
			const Ray& ray, const std::shared_ptr<Sampler>& sampler, Intersection& isect, int depth) const;

		virtual std::shared_ptr<Film> get_film() const;

		virtual std::shared_ptr<Camera> get_camera() const;

		int num_samples{};
		std::shared_ptr<Scene> scene;
		std::vector<std::shared_ptr<Sampler>> samplers;
		float bias = 0.00001f;
	};
}
