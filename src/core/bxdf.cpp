#include "bxdf.h"

#include <glm/gtc/constants.inl>
#include "../math/math.h"

float pbr::BxDF::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return math::same_hemisphere(wo, wi) ? math::abs_cos_theta(wi) * glm::one_over_pi<float>() : 0.f;
}
