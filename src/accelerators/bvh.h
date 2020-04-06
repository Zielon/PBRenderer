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

			// Compute bbox of all primitives in the BVH node.
			for (int i = node->start; i < node->end; ++i)
				node->bbox.extend(primitives[i].getBBox());

			// Leaf condition and finish recursion.
			if (node->elements == 1)
			{
				return;
			}

			int extent = node->bbox.maximum_extent();

			auto middle = (node->start + node->end) / 2;

			auto mid = primitives.begin() + middle;
			auto begin = primitives.begin() + node->start;
			auto end = primitives.begin() + node->end;

			node->left = std::make_unique<Node>(node->start, middle);
			node->right = std::make_unique<Node>(middle, node->end);

			std::nth_element(begin, mid, end, [extent](T& a, T& b){
				return a.getBBox().centroid()[extent] < b.getBBox().centroid()[extent];
			});

			middle_split(node->left.get());
			middle_split(node->right.get());
		}

		void sah_split(){ }

		void equal_split(){}
	};
}
