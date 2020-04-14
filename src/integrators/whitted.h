#pragma once

#include "../core/integrator.h"

namespace pbr
{
	class WhittedIntegrator : public Integrator
	{
	public:
		WhittedIntegrator(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Film>& film)
			: Integrator(scene, film){}

		void render() override;
	};
}
