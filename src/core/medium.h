#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "../core/uniform_sampler.h"
#include "../geometry/ray.h"
#include "phase_function.h"

namespace pbr
{
	class Interaction{};

	class Medium
	{
	public:
		// Medium Interface
		virtual ~Medium(){}

		virtual glm::vec3 tr(const Ray& ray, Sampler& sampler) const = 0;

		virtual glm::vec3 sample(const Ray& ray, Sampler& sampler, Interaction* mi) const = 0;
	};
}
