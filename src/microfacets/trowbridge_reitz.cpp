#include "trowbridge_reitz.h"

#include "../math/math.h"

float pbr::TrowbridgeReitz::D(const glm::vec3& wh) const{

	float tan2Theta = math::tan2_theta(wh);
	if (std::isinf(tan2Theta)) return 0.;
	const float cos4Theta = math::cos2_theta(wh) * math::cos2_theta(wh);
	float e = (math::cos2_phi(wh) / (alpha * alpha) + math::sin2_phi(wh) / (alpha * alpha)) * tan2Theta;
	return 1.f / (glm::pi<float>() * alpha * alpha * cos4Theta * (1 + e) * (1 + e));
}

float pbr::TrowbridgeReitz::lambda(const glm::vec3& w) const{

	float abs_tan_theta = std::abs(math::tan_theta(w));
	if (std::isinf(abs_tan_theta)) return 0.;
	float a = std::sqrt(math::cos2_phi(w) * alpha * alpha + math::sin2_phi(w) * alpha * alpha);
	float alpha2_tan2_theta = (a * abs_tan_theta) * (a * abs_tan_theta);
	return (-1 + std::sqrt(1.f + alpha2_tan2_theta)) / 2;
}

glm::vec3 pbr::TrowbridgeReitz::sample_wh(const glm::vec3& wo, const glm::vec2& u){

	float phi = (2 * glm::pi<float>()) * u[1];
	float tan_theta2 = alpha * alpha * u[0] / (1.0f - u[0]);
	float cos_theta = 1 / std::sqrt(1 + tan_theta2);
	float sin_theta = std::sqrt(std::max(0.f, 1.f - cos_theta * cos_theta));
	glm::vec3 wh = math::spherical_direction(sin_theta, cos_theta, phi);
	if (!math::same_hemisphere(wo, wh)) wh = -wh;

	return wh;
}
