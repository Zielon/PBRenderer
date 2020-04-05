#pragma once

#include "../core/aggregate.h"
#include "../geometry/ray.h"
#include "../geometry/intersection.h"

namespace pbr
{
	enum class Split { SAH, Middle };

	template <typename T>
	class BVH : public Aggregate
	{
	public:
		void add(T object){ }

		void build(Split type){ }

		bool intersect(const Ray& ray, Intersection& intersection){ }

	private:
		void middle_split(){ }

		void sah_split(){ }
	};
}
