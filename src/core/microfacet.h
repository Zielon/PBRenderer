#pragma once

#include <glm/glm.hpp>

namespace pbr
{
	class UniformSampler;

	class MicrofacetDistribution
	{
	public:
		MicrofacetDistribution() = default;

		virtual ~MicrofacetDistribution() = default;

		/*
		 * Anisotropic microfacet distribution function
		 */
		virtual float D(const glm::vec3& wh) const = 0;

		/*
		 * Shadowing-masking functions which measures invisible masked microfacet area per visible microfacet area
		 */
		virtual float lambda(const glm::vec3& w) const = 0;

		virtual glm::vec3 sample_wh(const glm::vec3& wo, const glm::vec2& u) const = 0;

		float pdf(const glm::vec3& wo, const glm::vec3& wh) const;

		/*
		 * Gives a fraction of the total microfacet area that is visible in the given direction
		 */
		float G1(const glm::vec3& w) const{

			return 1 / (1 + lambda(w));
		}

		/*
		 * Fraction of visible microsurfaces from the incoming and outcoming direction
		 */
		virtual float G(const glm::vec3& wo, const glm::vec3& wi) const{

			return 1 / (1 + lambda(wo) + lambda(wi));
		}

	protected:
		MicrofacetDistribution(bool sample_visible_area): sample_visible_normals(sample_visible_area){};

		const bool sample_visible_normals;
	};
}
