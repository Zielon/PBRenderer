#include "ggx.h"

float pbr::GGX::D(const glm::vec3& wh) const{

	auto cos_theta = math::cos_theta(wh);

	if (cos_theta > 0.0f)
	{
		auto temp = alpha * alpha + math::tan2_theta(wh);
		return alpha * alpha / (glm::pi<float>() * cos_theta * cos_theta * cos_theta * cos_theta * temp * temp);
	}

	return 0.0f;
}

float pbr::GGX::G1(const glm::vec3& wi, const glm::vec3& wh) const{

	if (dot(wi, wh) / math::cos_theta(wi) > 0.0f)
		return 2.0f / (1.0f + glm::sqrt(1.0f + alpha * alpha * math::tan2_theta(wi)));

	return 0.0f;
}

float pbr::GGX::pdf(const glm::vec3& wo, const glm::vec3& wh) const{

	return glm::abs(G1(wo, wh) * dot(wo, wh) * D(wh) / math::cos_theta(wo));
}

/*
 * A Simpler and Exact Sampling Routine for the GGX by Eric Heitz
 * https://hal.archives-ouvertes.fr/hal-01509746/document
 */
glm::vec3 pbr::GGX::sample_wh(const glm::vec3& w, const glm::vec2& u){

	auto wi = math::cos_theta(w) < 0.0f ? -w : w;
	auto stretched_wi = normalize(glm::vec3(alpha * wi.x, alpha * wi.y, wi.z));
	auto t1 = math::cos_theta(stretched_wi) < 0.9999f ? normalize(cross(stretched_wi, glm::vec3(0.0f, 0.0f, 1.0f)))
		          : glm::vec3(1.0f, 0.0f, 0.0f);
	auto t2 = cross(t1, stretched_wi);
	auto u1 = u.x;
	auto u2 = u.y;
	auto a = 1.0f / (1.0f + math::cos_theta(stretched_wi));
	auto r = glm::sqrt(u1);
	auto phi = u2 < a ? u2 / a * glm::pi<float>() : glm::pi<float>() + (u2 - a) / (1.0f - a) * glm::pi<float>();
	auto p1 = r * glm::cos(phi);
	auto p2 = r * glm::sin(phi) * (u2 < a ? 1.0f : math::cos_theta(stretched_wi));
	auto p3 = glm::sqrt(glm::max(0.0f, 1.0f - p1 * p1 - p2 * p2));
	auto n = p1 * t1 + p2 * t2 + p3 * stretched_wi;
	return normalize(glm::vec3(alpha * n.x, alpha * n.y, glm::max(0.0f, n.z)));
}
