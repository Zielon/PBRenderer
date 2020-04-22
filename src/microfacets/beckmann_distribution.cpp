#include "beckmann_distribution.h"
#include "../math/math.h"

float pbr::BeckmannDistribution::D(const glm::vec3& wh) const{

	const auto tan = math::tan2_theta(wh);

	if (std::isinf(tan)) return 0.f;

	return std::exp(-tan * (math::cos2_phi(wh) / (alphax * alphax) + math::sin2_phi(wh) / (alphay * alphay))) / (glm::pi
		<float>() * alphax * alphay * math::cos2_theta(wh) * math::cos2_theta(wh));
}

float pbr::BeckmannDistribution::lambda(const glm::vec3& w) const{

	const float abs_tan_theta = std::abs(math::tan_theta(w));

	if (std::isinf(abs_tan_theta)) return 0.f;

	const float alpha = std::sqrt(math::cos2_phi(w) * alphax * alphax + math::sin2_phi(w) * alphay * alphay);
	const float a = 1 / (alpha * abs_tan_theta);

	if (a >= 1.6f) return 0;

	return (1 - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}

glm::vec3 pbr::BeckmannDistribution::sample_wh(const glm::vec3& wo, const glm::vec2& u) const{

	float tan2Theta, phi;
	if (alphax == alphay)
	{
		float log_sample = std::log(1 - u[0]);
		tan2Theta = -alphax * alphax * log_sample;
		phi = u[1] * 2 * glm::pi<float>();
	}
	else
	{
		float log_sample = std::log(1 - u[0]);
		phi = std::atan(alphay / alphax * std::tan(2 * glm::pi<float>() * u[1] + 0.5f * glm::pi<float>()));
		if (u[1] > 0.5f) phi += glm::pi<float>();
		float sin_phi = std::sin(phi), cosPhi = std::cos(phi);
		float alphax2 = alphax * alphax, alphay2 = alphay * alphay;
		tan2Theta = -log_sample /
			(cosPhi * cosPhi / alphax2 + sin_phi * sin_phi / alphay2);
	}

	float cos_theta = 1 / std::sqrt(1 + tan2Theta);
	float sin_theta = std::sqrt(std::max(float(0), 1 - cos_theta * cos_theta));
	glm::vec3 wh = math::spherical_direction(sin_theta, cos_theta, phi);
	if (!math::same_hemisphere(wo, wh)) wh = -wh;
	return wh;
}
