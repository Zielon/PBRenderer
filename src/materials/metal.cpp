#include "metal.h"

#include "../bxdfs/fresnel.h"
#include "../core/reflection.h"
#include "../geometry/triangle.h"
#include "../bxdfs/microfacet_reflection.h"
#include "../bxdfs/lambertian_reflection.h"
#include "../microfacets/trowbridge_reitz.h"

void pbr::MetalMaterial::compute_BxDF(Intersection& intersection, TransportMode mode) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	//GOLD
	auto eta = glm::vec3(0.13046, 0.48358, 1.5599); // Refractive index
	auto k = glm::vec3(3.7391, 2.2124, 1.9255);     // Extinction coefficient
	auto r = glm::vec3(1.f);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);

	auto fresnel = std::make_shared<FresnelConductor>(glm::vec3(1.f), eta, k);
	auto distribution = std::make_shared<TrowbridgeReitz>(.2f);

	intersection.bsdf->add(std::make_shared<LambertianReflection>(glm::vec3(1.f, 0.766f, 0.336f)));
	intersection.bsdf->add(std::make_shared<MicrofacetReflection>(distribution, r, fresnel));
}
