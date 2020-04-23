#pragma once

#include "../core/integrator.h"

namespace pbr
{
	class WhittedIntegrator : public Integrator
	{
	public:
		WhittedIntegrator(std::shared_ptr<Scene> scene, int num_samples)
			: Integrator(std::move(scene), num_samples){ }

	private:
		glm::vec3 Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const override;
	};
}
