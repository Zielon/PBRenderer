#pragma once

#include "../core/bxdf.h"
#include "fresnel.h"

#include <utility>

namespace pbr
{
	class SpecularReflection : public BxDF
	{
	public:
		SpecularReflection(const glm::vec3& r, std::shared_ptr<Fresnel> fresnel)
			: BxDF(BxDFType(REFLECTION | SPECULAR)), r(r), fresnel(std::move(fresnel)){}

		glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const override;

		glm::vec3 sample_f(const glm::vec3& wo, glm::vec3* wi,
		                   const glm::vec2& sample, float* pdf,
		                   BxDFType* sampledType = nullptr) const override;

		float pdf(const glm::vec3& wo, const glm::vec3& wi) const override;

	private:
		const glm::vec3 r;
		const std::shared_ptr<Fresnel> fresnel;
	};
}
