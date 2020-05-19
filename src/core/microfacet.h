#pragma once

#include <glm/glm.hpp>
#include "../math/math.h"

namespace pbr
{
	class UniformSampler;

	class MicrofacetDistribution
	{
	public:
		MicrofacetDistribution() = default;

		virtual ~MicrofacetDistribution() = default;

		virtual float D(const glm::vec3& wh) const = 0;

		virtual float G1(const glm::vec3& wi, const glm::vec3& wh) const = 0;

		virtual glm::vec3 sample_wh(const glm::vec3& wo, const glm::vec2& u) = 0;

		virtual float pdf(const glm::vec3& wo, const glm::vec3& wh) const = 0;

		static float roughness_to_alpha(float roughness);
	};
}
