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
		glm::vec3 process(const Ray& ray, int depth) const override;
	};
}
