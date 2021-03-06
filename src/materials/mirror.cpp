#include "mirror.h"

#include "../geometry/triangle.h"
#include "../bxdfs/specular_reflection.h"

void pbr::MirrorMaterial::compute_BxDF(Intersection& intersection, TransportMode mode) const{

	Triangle* triangle = const_cast<Triangle*>(intersection.triangle);

	auto r = kr->evaluate(intersection);

	intersection.bsdf = std::make_shared<BSDF>(intersection, triangle->scene_object);
	intersection.bsdf->add(std::make_shared<SpecularReflection>(r, std::make_shared<FresnelMirror>()));
}
