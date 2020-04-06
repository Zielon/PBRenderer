#pragma once

#include "node.h"
#include "../core/aggregate.h"
#include "../geometry/ray.h"
#include "../geometry/intersection.h"

namespace pbr
{
	enum class Split { SAH, MIDDLE, EQUAL };

	template <typename T>
	class BVH : public Aggregate
	{
	public:
		void add(T object){
			primitives.push_back(object);
		}

		void build(Split type){

			switch (type)
			{
			case Split::MIDDLE:
				middle_split();
				break;
			case Split::EQUAL:
				equal_split();
				break;
			case Split::SAH:
				sah_split();
				break;
			}
		}

		bool intersect(const Ray& ray, Intersection& intersection){ }

	private:
		std::vector<T> primitives;
		std::shared_ptr<Node> root;

		void middle_split(){ }

		void sah_split(){ }

		void equal_split(){}
	};
}
