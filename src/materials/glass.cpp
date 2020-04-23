#include "glass.h"

#include "../core/reflection.h"
#include "../geometry/triangle.h"
#include "../bxdfs/fresnel.h"
#include "../bxdfs/specular_transmission.h"
#include "../bxdfs/specular_reflection.h"

void pbr::GlassMaterial::compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);

	float eta = index->evaluate(intersection);
	glm::vec3 R = kr->evaluate(intersection);
	glm::vec3 T = kt->evaluate(intersection);

	auto fresnel = std::make_shared<FresnelDielectric>(1.f, eta);

	intersection.bsdf->add(std::make_shared<SpecularReflection>(R, fresnel));
	intersection.bsdf->add(std::make_shared<SpecularTransmission>(T, 1.f, eta, mode));
}
