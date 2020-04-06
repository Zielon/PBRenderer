#pragma once

#include <algorithm>

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
				root = std::make_unique<Node>(0, primitives.size());
				middle_split(root.get());
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

		void middle_split(Node* node){

			for (int i = node->offset; i < node->length; ++i)
				node->bbox.extend(primitives[i].getBBox());

			// Leaf condition and finish recursion.
			if (node->elements <= 1)
			{
				return;
			}

			int slab = node->bbox.maximum_slab();

			auto middle = (node->offset + node->length) / 2;

			auto mid = primitives.begin() + middle;
			auto begin = primitives.begin() + node->offset;
			auto end = primitives.begin() + node->length;

			node->left = std::make_unique<Node>(node->offset, middle);
			node->right = std::make_unique<Node>(middle, node->length);

			std::nth_element(begin, mid, end, [slab](T& a, T& b){
				return a.getBBox().centroid()[slab] < b.getBBox().centroid()[slab];
			});

			middle_split(node->left.get());
			middle_split(node->right.get());
		}

		void sah_split(){ }

		void equal_split(){}
	};
}
