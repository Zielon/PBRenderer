#include "microfacet_transmission.h"

glm::vec3 pbr::MicrofacetTransmission::f(const glm::vec3& wo, const glm::vec3& wi) const{

	if (math::same_hemisphere(wo, wi)) return glm::vec3(0);

	const float cos_theta_o = math::cos_theta(wo);
	const float cos_theta_i = math::cos_theta(wi);
	if (cos_theta_i == 0 || cos_theta_o == 0) return glm::vec3(0);

	float eta = math::cos_theta(wo) > 0 ? (eta_b / eta_a) : (eta_a / eta_b);
	glm::vec3 wh = normalize(wo + wi * eta);
	if (wh.z < 0) wh = -wh;

	glm::vec3 f = fresnel->evaluate(dot(wo, wh));

	const float sqrt_denom = dot(wo, wh) + eta * dot(wi, wh);
	const float factor = (mode == TransportMode::Radiance) ? (1 / eta) : 1;

	return (glm::vec3(1.f) - f) * t *
		std::abs(distribution->D(wh) * distribution->G(wo, wi) * eta * eta *
			std::abs(dot(wi, wh)) * std::abs(dot(wo, wh)) * factor * factor /
			(cos_theta_i * cos_theta_o * sqrt_denom * sqrt_denom));

}

glm::vec3 pbr::MicrofacetTransmission::sample_f(const glm::vec3& wo, glm::vec3* wi, const glm::vec2& sample, float* _pdf,
                                                BxDFType* sampledType) const{

	if (wo.z == 0) return glm::vec3(0.f);
	glm::vec3 wh = distribution->sample_wh(wo, sample);
	if (dot(wo, wh) < 0) return glm::vec3(0.f);
	float eta = math::cos_theta(wo) > 0 ? (eta_a / eta_b) : (eta_b / eta_a);
	glm::vec3 result = math::refract(wo, wh, eta);
	wi = &result;
	*_pdf = pdf(wo, *wi);

	return f(wo, *wi);
}

float pbr::MicrofacetTransmission::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	if (math::same_hemisphere(wo, wi)) glm::vec3(0.f);
	float eta = math::cos_theta(wo) > 0 ? (eta_b / eta_a) : (eta_a / eta_b);
	const glm::vec3 wh = normalize(wo + wi * eta);
	const float sqrt_denom = dot(wo, wh) + eta * dot(wi, wh);
	const float dwh_dwi = std::abs((eta * eta * dot(wi, wh)) / (sqrt_denom * sqrt_denom));

	return distribution->pdf(wo, wh) * dwh_dwi;
}
