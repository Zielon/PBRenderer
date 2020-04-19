#include "lambertian.h"
#include "../core/bxdf.h"

void pbr::LambertianMaterial::compute(Intersection& intersection, TransportMode mode, bool multiple_lobes) const{

	auto bsdf = std::make_shared<BSDF>(intersection);
	bsdf->add(std::make_shared<LambertianReflection>(kd->evaluate(intersection)));
	intersection.bsdf = bsdf;
}
