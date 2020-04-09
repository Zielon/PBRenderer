#include "scene.h"

bool pbr::Scene::intersect(const Ray& ray, Intersection& intersection) const{

	return bvh->intersect(ray, intersection);
}
