#include "lambertian.h"

#include "../bxdfs/lambertian_reflection.h"

void pbr::LambertianMaterial::compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const{

	intersection.bsdf = std::make_shared<BSDF>(intersection);
	intersection.bsdf->add(std::make_shared<LambertianReflection>(kd->evaluate(intersection)));
}
