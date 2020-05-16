#include "metal.h"

#include "../bxdfs/fresnel.h"
#include "../microfacets/beckmann_distribution.h"
#include "../core/reflection.h"
#include "../geometry/triangle.h"
#include "../bxdfs/microfacet_reflection.h"
#include "../bxdfs/lambertian_reflection.h"

void pbr::MetalMaterial::compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	//GOLD
	auto eta = glm::vec3(3.1099, 1.1851, 0.9132); // Refractive index
	auto k = glm::vec3(2.9567, 2.2785, 1.5698);   // Extinction coefficient
	auto r = glm::vec3(1.f);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);

	auto fresnel = std::make_shared<FresnelConductor>(glm::vec3(1.f), eta, k);
	auto distribution = std::make_shared<BeckmannDistribution>(MicrofacetDistribution::roughness_to_alpha(.01f),
	                                                           MicrofacetDistribution::roughness_to_alpha(.02f));

	intersection.bsdf->add(std::make_shared<LambertianReflection>(glm::vec3(1.f, 0.766f, 0.336f)));
	intersection.bsdf->add(std::make_shared<MicrofacetReflection>(distribution, r, fresnel));
}
