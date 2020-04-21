#include "lambertian.h"

#include "../geometry/triangle.h"
#include "../bxdfs/lambertian_reflection.h"

void pbr::LambertianMaterial::compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);
	intersection.bsdf->add(std::make_shared<LambertianReflection>(kd->evaluate(intersection)));
}
