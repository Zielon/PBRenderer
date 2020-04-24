#include "plastic.h"

#include "../core/reflection.h"
#include "../geometry/triangle.h"
#include "../bxdfs/fresnel.h"
#include "../microfacets/beckmann_distribution.h"
#include "../bxdfs/lambertian_reflection.h"
#include "../bxdfs/microfacet_reflection.h"

inline float roughness_to_alpha(float roughness){
	roughness = std::max(roughness, float(1e-3));
	float x = std::log(roughness);
	return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x + 0.000640711f * x * x * x * x;
}

void pbr::PlasticMaterial::compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);

	auto kd = glm::vec3(0.55f);
	auto r = glm::vec3(0.25f);
	auto fresnel = std::make_shared<FresnelDielectric>(1.5f, 1.f);
	auto distribution = std::make_shared<BeckmannDistribution>(roughness_to_alpha(.01f), roughness_to_alpha(.01f));

	intersection.bsdf->add(std::make_shared<LambertianReflection>(kd));
	intersection.bsdf->add(std::make_shared<MicrofacetReflection>(distribution, r, fresnel));
}
