#include "specular_transmission.h"

#include "../math/math.h"

glm::vec3 pbr::SpecularTransmission::sample_f(
	const glm::vec3& wo, glm::vec3* wi, const glm::vec2& sample, float* pdf, BxDFType* sampledType) const{

	float eta = math::cos_theta(wo) > 0 ? (eta_a / eta_b) : (eta_b / eta_a);

	glm::vec3 wh = math::face_forward(glm::vec3(0, 0, 1), wo);

	if (!math::refract(wo, wh, eta, wi))
		return glm::vec3(0.f);

	*pdf = 1.f;
	glm::vec3 ft = t * (glm::vec3(1.f) - fresnel->evaluate(math::cos_theta(*wi)));

	if (mode == TransportMode::Radiance)
		ft *= eta;

	return ft / math::abs_cos_theta(*wi);
}

glm::vec3 pbr::SpecularTransmission::f(const glm::vec3& wo, const glm::vec3& wi) const{

	return glm::vec3(0.f);
}

float pbr::SpecularTransmission::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return 0.f;
}
