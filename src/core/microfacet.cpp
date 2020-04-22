#include "microfacet.h"

#include "../math/math.h"

float pbr::MicrofacetDistribution::pdf(const glm::vec3& wo, const glm::vec3& wh) const{

	if (sample_visible_normals)
		return D(wh) * G1(wo) * std::abs(dot(wo, wh)) / math::abs_cos_theta(wo);

	return D(wh) * math::abs_cos_theta(wh);
}
