#include "beckmann_distribution.h"
#include "../math/math.h"

float pbr::BeckmannDistribution::D(const glm::vec3& wh) const{

	const auto tan = math::tan2_theta(wh);

	if (std::isinf(tan)) return 0.f;

	return std::exp(-tan * (math::cos2_phi(wh) / (alpha * alpha) + math::sin2_phi(wh) / (alpha * alpha))) /
		(glm::pi<float>() * alpha * alpha * math::cos2_theta(wh) * math::cos2_theta(wh));
}

float pbr::BeckmannDistribution::lambda(const glm::vec3& w) const{

	const float abs_tan_theta = std::abs(math::tan_theta(w));

	if (std::isinf(abs_tan_theta)) return 0.f;

	const float alphaf = std::sqrt(math::cos2_phi(w) * alpha * alpha + math::sin2_phi(w) * alpha * alpha);
	const float a = 1 / (alphaf * abs_tan_theta);

	if (a >= 1.6f) return 0;

	return (1 - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}

glm::vec3 pbr::BeckmannDistribution::sample_wh(const glm::vec3& wo, const glm::vec2& u){

	alpha = (1.2f - 0.2f * glm::sqrt(glm::abs(math::cos_theta(wo)))) * alpha;

	float tan2Theta, phi;
	float log_sample = std::log(1 - u[0]);
	tan2Theta = -alpha * alpha * log_sample;
	phi = u[1] * 2 * glm::pi<float>();
	float cos_theta = 1 / std::sqrt(1 + tan2Theta);
	float sin_theta = std::sqrt(std::max(0.f, 1.f - cos_theta * cos_theta));
	glm::vec3 wh = math::spherical_direction(sin_theta, cos_theta, phi);

	if (!math::same_hemisphere(wo, wh)) wh = -wh;

	return wh;
}
