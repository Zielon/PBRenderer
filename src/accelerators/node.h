#pragma once

#include <memory>

#include "../geometry/bbox.h"

namespace pbr
{
	class Node final
	{
	public:
		BBox bbox;
		int start;
		int end;
		int elements;
		std::unique_ptr<Node> left{nullptr};
		std::unique_ptr<Node> right{nullptr};

		Node() = default;

		Node(int start, int end): start(start), end(end), elements(end - start){}

		Node(const BBox& bbox, int start, int end): bbox(bbox), start(start), end(end),
		                                            elements(end - start){}

		bool intersect(const Ray& ray) const{

			const auto inv = 1.0f / ray.d;
			return bbox.intersect(ray, inv);
		}
	};
}
