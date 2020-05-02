#pragma once

#include "scene.h"
#include "film.h"
#include "uniform_sampler.h"

#include <glm/glm.hpp>
#include <atomic>

namespace pbr
{
	class Integrator
	{
	public:
		Integrator(std::shared_ptr<Scene> scene, int num_samples, std::string name);

		virtual ~Integrator() = default;

		virtual void render(std::atomic<float>& progress);

		virtual std::shared_ptr<Light> select_light(float u, float* pdf) const;

		virtual glm::vec3 direct_illumination(
			const Intersection& intersection,
			const std::shared_ptr<Light>& light,
			const std::shared_ptr<Sampler>& sampler) const;

		virtual glm::vec3 reflect(
			const Ray& ray,
			const std::shared_ptr<Sampler>& sampler,
			Intersection& isect, int depth) const;

		virtual glm::vec3 transmit(
			const Ray& ray,
			const std::shared_ptr<Sampler>& sampler,
			Intersection& isect, int depth) const;

	protected:
		virtual glm::vec3 Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const = 0;

		virtual std::shared_ptr<Film> get_film() const;

		virtual std::shared_ptr<Camera> get_camera() const;

		std::string name;
		int num_samples{};
		int max_depth = 5;
		std::shared_ptr<Scene> scene;
		std::vector<std::shared_ptr<Sampler>> samplers;
		float ray_epsilon = 0.00001f;
	};
}
