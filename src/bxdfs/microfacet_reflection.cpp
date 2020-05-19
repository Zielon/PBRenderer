#include "microfacet_reflection.h"

glm::vec3 pbr::MicrofacetReflection::f(const glm::vec3& wo, const glm::vec3& wi) const{

	glm::vec3 wh = normalize(wi + wo);
	auto m_ior_n = 1.5f;
	auto no_over_ni = math::cos_theta(wo) > 0.0f ? 1.0f / m_ior_n : m_ior_n;
	auto F = fresnel->evaluate(glm::abs(dot(wo, wh)), 1.f / no_over_ni);
	auto nominator = F * distribution->D(wh) * distribution->G1(wi, wh) * distribution->G1(wo, wh);
	return nominator / glm::abs(4.0f * math::cos_theta(wi) * math::cos_theta(wo));
}

glm::vec3 pbr::MicrofacetReflection::sample_f(
	const glm::vec3& wo, glm::vec3* wi, const glm::vec2& u, float* pdf, BxDFType* sampledType) const{

	const glm::vec3 wh = distribution->sample_wh(wo, u);

	*wi = reflect(-wo, wh);
	*pdf = distribution->pdf(wo, wh) / (4.f * dot(wo, wh));

	return f(wo, *wi);
}

float pbr::MicrofacetReflection::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	if (!math::same_hemisphere(wo, wi)) return 0;
	glm::vec3 wh = normalize(wi + wo);
	return glm::abs(distribution->pdf(wo, wh) / (4 * dot(wo, wh)));
}
