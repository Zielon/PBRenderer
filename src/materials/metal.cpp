#include "metal.h"

#include "../bxdfs/fresnel.h"
#include "../core/reflection.h"
#include "../geometry/triangle.h"
#include "../bxdfs/microfacet_reflection.h"
#include "../microfacets/ggx.h"

void pbr::MetalMaterial::compute_BxDF(Intersection& intersection, TransportMode mode) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	// Fresnel for conductor GOLD
	auto eta_i = glm::vec3(0.13046, 0.48358, 1.5599);
	auto eta_t = glm::vec3(3.7391, 2.2124, 1.9255);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);

	auto fresnel = std::make_shared<FresnelConductor>(eta_i, eta_t, glm::vec3(1.f));
	auto distribution = std::make_shared<GGX>(.15f);

	intersection.bsdf->add(std::make_shared<MicrofacetReflection>(distribution, fresnel));
}
