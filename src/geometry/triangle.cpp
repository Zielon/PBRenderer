#include "triangle.h"

bool pbr::Triangle::intersect(const Ray& ray, Intersection& intersection){

	return false;
}

pbr::BBox pbr::Triangle::getBBox(){

	return bbox;
}
