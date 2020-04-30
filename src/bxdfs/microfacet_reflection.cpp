#include "microfacet_reflection.h"

glm::vec3 pbr::MicrofacetReflection::f(const glm::vec3& wo, const glm::vec3& wi) const{

	float cos_theta_o = math::abs_cos_theta(wo);
	float cos_theta_i = math::abs_cos_theta(wi);
	glm::vec3 wh = wi + wo;

	if (cos_theta_i == 0 || cos_theta_o == 0) return glm::vec3(0.);
	if (wh.x == 0 && wh.y == 0 && wh.z == 0) return glm::vec3(0.);
	wh = normalize(wh);
	glm::vec3 f = fresnel->evaluate(dot(wi, math::face_forward(wh, glm::vec3(0, 0, 1))));

	return r * distribution->D(wh) * distribution->G(wo, wi) * f / (4 * cos_theta_i * cos_theta_o);
}

glm::vec3 pbr::MicrofacetReflection::sample_f(
	const glm::vec3& wo, glm::vec3* wi, const glm::vec2& sample, float* pdf, BxDFType* sampledType) const{

	if (wo.z == 0) return glm::vec3(0.f);
	const glm::vec3 wh = normalize(distribution->sample_wh(wo, sample));
	if (dot(wo, wh) < 0) glm::vec3(0.f);
	*wi = math::reflect(wo, wh);
	if (!math::same_hemisphere(wo, *wi))
		return glm::vec3(0.f);

	*pdf = distribution->pdf(wo, wh) / (4 * dot(wo, wh));

	return f(wo, *wi);
}

float pbr::MicrofacetReflection::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	if (!math::same_hemisphere(wo, wi)) return 0;
	glm::vec3 wh = normalize(wo + wi);
	return distribution->pdf(wo, wh) / (4 * dot(wo, wh));
}
