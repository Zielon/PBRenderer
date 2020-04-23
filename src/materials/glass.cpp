#include "glass.h"

#include "../core/reflection.h"
#include "../geometry/triangle.h"
#include "../bxdfs/fresnel.h"
#include "../microfacets/beckmann_distribution.h"
#include "../bxdfs/microfacet_reflection.h"
#include "../bxdfs/microfacet_transmission.h"
#include "../bxdfs/specular_transmission.h"
#include "../bxdfs/specular_reflection.h"

inline float roughness_to_alpha(float roughness){
	roughness = std::max(roughness, float(1e-3));
	float x = std::log(roughness);
	return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x + 0.000640711f * x * x * x * x;
}

void pbr::GlassMaterial::compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);

	float eta = index->evaluate(intersection);
	float urough = u_roughness->evaluate(intersection);
	float vrough = v_roughness->evaluate(intersection);
	glm::vec3 R = kr->evaluate(intersection);
	glm::vec3 T = kt->evaluate(intersection);

	auto distribution = std::make_shared<BeckmannDistribution>(roughness_to_alpha(urough), roughness_to_alpha(vrough));
	auto fresnel = std::make_shared<FresnelDielectric>(1.f, eta);

	//intersection.bsdf->add(std::make_shared<MicrofacetTransmission>(distribution, T, 1.f, eta, mode));
	//intersection.bsdf->add(std::make_shared<MicrofacetReflection>(distribution, R, fresnel));

	intersection.bsdf->add(std::make_shared<SpecularReflection>(R, fresnel));
	intersection.bsdf->add(std::make_shared<SpecularTransmission>(T, 1.f, eta, mode));
}
