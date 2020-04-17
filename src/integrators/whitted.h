#pragma once

#include "../core/integrator.h"

namespace pbr
{
	class WhittedIntegrator : public Integrator
	{
	public:
		WhittedIntegrator(std::shared_ptr<Scene> scene)
			: Integrator(std::move(scene)){ }

	private:
		glm::vec3 Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const override;
	};
}
