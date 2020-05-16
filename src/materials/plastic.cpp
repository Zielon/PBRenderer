#include "plastic.h"

#include "../core/reflection.h"
#include "../geometry/triangle.h"
#include "../bxdfs/fresnel.h"
#include "../microfacets/beckmann_distribution.h"
#include "../bxdfs/lambertian_reflection.h"
#include "../bxdfs/microfacet_reflection.h"

void pbr::PlasticMaterial::compute_BxDF(Intersection& intersection, TransportMode mode) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);

	auto kd = glm::vec3(0.235294, 0.67451, 0.843137);
	auto r = glm::vec3(0.05f);
	auto fresnel = std::make_shared<FresnelDielectric>(1.5f, 1.f);
	auto distribution = std::make_shared<BeckmannDistribution>(MicrofacetDistribution::roughness_to_alpha(.01f));

	intersection.bsdf->add(std::make_shared<LambertianReflection>(kd));
	intersection.bsdf->add(std::make_shared<MicrofacetReflection>(distribution, r, fresnel));
}
