#include "plastic.h"

#include "../core/reflection.h"
#include "../geometry/triangle.h"
#include "../bxdfs/fresnel.h"
#include "../bxdfs/lambertian_reflection.h"
#include "../bxdfs/microfacet_reflection.h"
#include "../microfacets/ggx.h"

void pbr::PlasticMaterial::compute_BxDF(Intersection& intersection, TransportMode mode) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);

	auto kd = glm::vec3(51.f / 255.f, 51.f / 255.f, 153.f / 255.f);
	auto fresnel = std::make_shared<FresnelDielectric>(1.5f, 1.f);
	auto distribution = std::make_shared<GGX>(0.55f);

	intersection.bsdf->add(std::make_shared<LambertianReflection>(kd));
	intersection.bsdf->add(std::make_shared<MicrofacetReflection>(distribution, fresnel));
}
