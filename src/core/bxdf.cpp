#include "bxdf.h"

#include <glm/gtc/constants.hpp>

#include "../math/math.h"

glm::vec3 pbr::BxDF::sample_f(
	const glm::vec3& wo, glm::vec3* wi, const glm::vec2& sample, float* _pdf, BxDFType* sampledType) const{

	*wi = math::cosine_sample_hemisphere(sample);
	if (wo.z < 0) wi->z *= -1;
	*_pdf = pdf(wo, *wi);

	return f(wo, *wi);
}

float pbr::BxDF::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return math::same_hemisphere(wo, wi) ? math::abs_cos_theta(wi) * glm::one_over_pi<float>() : 0.f;
}

bool pbr::BxDF::matches_flags(const BxDFType t) const{

	return (type & t) == type;
}
