#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../core/microfacet.h"

namespace pbr
{
	class TrowbridgeReitz : public MicrofacetDistribution
	{
	public:
		TrowbridgeReitz(float alphax, float alphay) : MicrofacetDistribution(false), alphax(alphax),
		                                              alphay(alphay){};

		float D(const glm::vec3& wh) const override;

		float lambda(const glm::vec3& w) const override;

		glm::vec3 sample_wh(const glm::vec3& wo, const glm::vec2& u) const override;

	private:
		const float alphax{};
		const float alphay{};
	};
}
