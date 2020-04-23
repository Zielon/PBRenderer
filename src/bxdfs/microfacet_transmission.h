#pragma once

#include <utility>
#include "fresnel.h"
#include "../core/bxdf.h"
#include "../core/microfacet.h"

namespace pbr
{
	class MicrofacetTransmission : public BxDF
	{
	public:

		MicrofacetTransmission(
			std::shared_ptr<MicrofacetDistribution> distribution,
			const glm::vec3& t,
			float eta_a,
			float eta_b,
			TransportMode mode) :
			BxDF(BxDFType(TRANSMISSION | GLOSSY)), t(t), eta_a(eta_a), eta_b(eta_b),
			fresnel(std::make_shared<FresnelDielectric>(eta_a, eta_b)),
			mode(mode),
			distribution(std::move(distribution)){}

		glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const override;

		glm::vec3 sample_f(const glm::vec3& wo, glm::vec3* wi,
		                   const glm::vec2& sample, float* pdf,
		                   BxDFType* sampledType = nullptr) const override;

		float pdf(const glm::vec3& wo, const glm::vec3& wi) const override;

	private:
		const glm::vec3 t;
		const float eta_a, eta_b;
		const std::shared_ptr<FresnelDielectric> fresnel;
		const TransportMode mode;
		const std::shared_ptr<MicrofacetDistribution> distribution;
	};
}
