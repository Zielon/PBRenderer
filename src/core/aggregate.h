#pragma once
#include "../geometry/intersection.h"

namespace pbr
{
	class Aggregate
	{
	public:
		virtual ~Aggregate() = default;

		virtual bool intersect(const Ray& ray, Intersection& intersection) const = 0;
	};
}
