#pragma once

#include "../core/medium.h"

namespace pbr
{
	class HomogeneousMedium : public Medium
	{
	public:
		HomogeneousMedium(const glm::vec3& sigma_a, const glm::vec3& sigma_s, float g)
			: sigma_a(sigma_a),
			  sigma_s(sigma_s),
			  sigma_t(sigma_s + sigma_a),
			  g(g){}

		glm::vec3 tr(const Ray& ray, Sampler& sampler) const override;

		glm::vec3 sample(const Ray& ray, Sampler& sampler, Interaction* mi) const override;

	private:
		const glm::vec3 sigma_a;
		const glm::vec3 sigma_s;
		const glm::vec3 sigma_t;
		const float g;
	};
}
