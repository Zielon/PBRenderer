#pragma once

#include <glm/glm.hpp>

#include "../core/microfacet.h"

namespace pbr
{
	class GGX : public MicrofacetDistribution
	{
	public:
		GGX(float alpha) : alpha(alpha){};

		float D(const glm::vec3& wh) const override;

		float G1(const glm::vec3& wi, const glm::vec3& wh) const override;

		float pdf(const glm::vec3& wo, const glm::vec3& wh) const override;

		glm::vec3 sample_wh(const glm::vec3& wo, const glm::vec2& u) override;

	private:
		float alpha{};
	};
}
