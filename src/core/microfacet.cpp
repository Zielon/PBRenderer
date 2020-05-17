#include "microfacet.h"

#include "../math/math.h"

float pbr::MicrofacetDistribution::pdf(const glm::vec3& wo, const glm::vec3& wh) const{

	return D(wh) * math::abs_cos_theta(wh);
}

float pbr::MicrofacetDistribution::roughness_to_alpha(float roughness){

	roughness = std::max(roughness, float(1e-3));
	float x = std::log(roughness);
	return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x + 0.000640711f * x * x * x * x;
}
