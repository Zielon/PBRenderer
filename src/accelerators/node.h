#pragma once

#include <memory>

#include "../geometry/bbox.h"

namespace pbr
{
	class Node final
	{
	public:
		BBox bbox;
		int start{0};
		int end{0};
		int elements{};
		float distance{};
		bool isLeaf = false;

		std::shared_ptr<Node> left{nullptr};
		std::shared_ptr<Node> right{nullptr};

		Node() = default;

		Node(int start, int end): start(start), end(end), elements(end - start){}

		Node(const BBox& bbox, int start, int end):
			bbox(bbox), start(start), end(end), elements(end - start){}

		bool intersect(const Ray& ray){

			const auto inv = 1.0f / ray.d;
			return bbox.intersect(ray, inv, distance);
		}
	};
}
