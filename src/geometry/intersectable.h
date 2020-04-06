#pragma once

#include "ray.h"
#include "intersection.h"
#include "bbox.h"

namespace pbr
{
	class Intersectable
	{
	public:
		virtual ~Intersectable() = default;

		virtual bool intersect(const Ray& ray, Intersection& intersection) = 0;

		virtual BBox getBBox() = 0;
	};
}
