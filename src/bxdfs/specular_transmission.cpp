#include "specular_transmission.h"

glm::vec3 pbr::SpecularTransmission::sample_f(
	const glm::vec3& wo, glm::vec3* wi, const glm::vec2& sample, float* pdf, BxDFType* sampledType) const{

	bool entering = math::cos_theta(wo) > 0;
	float eta_i = entering ? eta_a : eta_b;
	float eta_t = entering ? eta_b : eta_a;

	glm::vec3 n = glm::vec3(0, 0, 1);
	n = (dot(n, wo) < 0.f) ? -n : n;

	auto r = math::refract(wo, n, eta_i / eta_t);
	wi = &r;

	*pdf = 1.f;
	glm::vec3 ft = t * (glm::vec3(1.f) - fresnel->evaluate(math::cos_theta(*wi)));

	if (mode == TransportMode::Radiance)
		ft *= (eta_i * eta_i) / (eta_t * eta_t);

	return ft / math::abs_cos_theta(*wi);
}

glm::vec3 pbr::SpecularTransmission::f(const glm::vec3& wo, const glm::vec3& wi) const{

	return glm::vec3(0.f);
}

float pbr::SpecularTransmission::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return 0.f;
}
