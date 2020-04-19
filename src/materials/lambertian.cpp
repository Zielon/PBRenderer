#include "lambertian.h"

void pbr::LambertianMaterial::compute(Intersection& intersection, TransportMode mode, bool multiple_lobes) const{
	
	intersection.bsdf = std::make_shared<BSDF>(intersection);

}
