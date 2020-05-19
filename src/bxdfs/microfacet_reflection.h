#pragma once

#include <utility>
#include "../core/bxdf.h"
#include "fresnel.h"
#include "../core/microfacet.h"

namespace pbr
{
	class MicrofacetReflection : public BxDF
	{
	public:
		MicrofacetReflection(
			std::shared_ptr<MicrofacetDistribution> distribution,
			std::shared_ptr<Fresnel> fresnel):
			BxDF(BxDFType(REFLECTION | GLOSSY)),
			fresnel(std::move(fresnel)),
			distribution(std::move(distribution)){}

		glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const override;

		glm::vec3 sample_f(const glm::vec3& wo, glm::vec3* wi,
		                   const glm::vec2& sample, float* pdf,
		                   BxDFType* sampledType = nullptr) const override;

		float pdf(const glm::vec3& wo, const glm::vec3& wi) const override;

	private:
		const std::shared_ptr<Fresnel> fresnel;
		const std::shared_ptr<MicrofacetDistribution> distribution;
	};
}
