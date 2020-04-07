#pragma once
#include "../geometry/intersection.h"

namespace pbr
{
	class Aggregate
	{
	public:
		virtual bool intersect(const Ray& ray, Intersection& intersection) = 0;
	};
}
