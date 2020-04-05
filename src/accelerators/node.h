#pragma once

#include <memory>

#include "../geometry/bbox.h"

namespace pbr
{
	class Node final
	{
	public:
		BBox bbox;
		int offset;
		int length;
		std::unique_ptr<Node> left{nullptr};
		std::unique_ptr<Node> right{nullptr};

		Node() = default;

		Node(int offset, int length): offset(offset), length(length){}

		Node(const BBox& bbox, int offset, int length): bbox(bbox), offset(offset), length(length){}
	};
}
