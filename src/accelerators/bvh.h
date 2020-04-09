#pragma once

#include <algorithm>
#include <queue>
#include <functional>

#include "node.h"
#include "../core/aggregate.h"
#include "../geometry/ray.h"
#include "../geometry/intersection.h"

namespace pbr
{
	/*
	 * Auxiliary structure for SAH method
	 */
	struct Bucket
	{
		Bucket() = default;

		Bucket(int count, BBox bbox) : count(count), bbox(bbox){}

		int count = 0;
		BBox bbox;
	};

	struct Comparator
	{
		bool operator()(const Node* lhs, const Node* rhs) const{
			return lhs->distance < rhs->distance;
		}
	};

	enum class Split { SAH, MIDDLE, EQUAL };

	template <typename T>
	class BVH final : public Aggregate
	{
	public:

		BVH() = default;

		void add(std::shared_ptr<T> object){
			primitives.push_back(object);
		}

		void build(Split type){

			initialize();

			switch (type)
			{
			case Split::MIDDLE:
				middle_split(root.get());
				break;
			case Split::EQUAL:
				equal_split(root.get());
				break;
			case Split::SAH:
				sah_split(root.get());
				break;
			}
		}

		bool intersect(const Ray& ray, Intersection& intersection) const override{

			std::priority_queue<Node*, std::vector<Node*>, Comparator> queue;

			queue.push(root.get());

			intersection.distance = std::numeric_limits<float>::max();

			while (!queue.empty())
			{
				auto node = queue.top();
				queue.pop();

				if (node->isLeaf)
				{
					for (int i = node->start; i < node->end; ++i)
						primitives[i]->intersect(ray, intersection);

					continue;
				}

				if (node->intersect(ray))
				{
					auto left = node->left.get();
					auto right = node->right.get();

					if (left->intersect(ray)) queue.push(left);
					if (right->intersect(ray)) queue.push(right);
				}
			}

			return intersection.distance < std::numeric_limits<float>::max();
		}

	private:
		std::vector<std::shared_ptr<T>> primitives;
		std::shared_ptr<Node> root;

		/**
		 * Simple median/middle split method which segments bounding volume centroids
		 * along the splitting axis.
		 */
		void middle_split(Node* node){

			// Compute bbox of all primitives in the BVH node.
			for (int i = node->start; i < node->end; ++i)
				node->bbox.extend(primitives[i]->get_bbox());

			// Leaf condition and finish recursion.
			if (node->elements < 64)
			{
				node->isLeaf = true;
				return;
			}

			int extent = node->bbox.maximum_extent();

			auto middle = (node->start + node->end) / 2;

			auto mid = primitives.begin() + middle;
			auto begin = primitives.begin() + node->start;
			auto end = primitives.begin() + node->end;

			node->left = std::make_unique<Node>(node->start, middle);
			node->right = std::make_unique<Node>(middle, node->end);

			std::nth_element(begin, mid, end, [extent](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b){
				return a->get_bbox().centroid()[extent] < b->get_bbox().centroid()[extent];
			});

			middle_split(node->left.get());
			middle_split(node->right.get());
		}

		/**
		 * More advance heuristic using surface area to find an axis which
		 * minimizes the SAH cost.
		 */
		void sah_split(Node* node){

			// Leaf condition and finish recursion.
			if (node->elements < 64)
			{
				node->isLeaf = true;
				return;
			}

			constexpr int n_buckets = 64;
			std::vector<Bucket> buckets(n_buckets);

			int axis = -1;
			int right_count = 0;
			BBox left_box;
			BBox right_box;
			auto cost = std::numeric_limits<float>::max();

			for (auto extent : {0, 1, 2})
			{
				// Fill buckets
				for (int i = node->start; i < node->end; ++i)
				{
					BBox bbox = primitives[i]->get_bbox();

					const auto centroid = bbox.centroid();
					const auto ratio = (centroid[extent] - node->bbox.min()[extent]) / node->bbox.diagonal()[extent];
					auto index = int(n_buckets * ratio);

					if (index == n_buckets) index = n_buckets - 1;

					++buckets[index].count;
					buckets[index].bbox.extend(bbox);
				}

				// Calculate cumulative distribution of bounding volumes [left to right]
				std::vector<BBox> cumulative_buckets(n_buckets);
				cumulative_buckets[0] = buckets[0].bbox;
				for (auto i = 1; i < n_buckets; ++i)
				{
					cumulative_buckets[i] = cumulative_buckets[i - 1];
					cumulative_buckets[i].extend(buckets[i].bbox);
				}

				BBox b1;
				int count1 = 0;

				// Calculate cost of splitting the bounding volumes [right to left]
				for (auto i = n_buckets - 1; i >= 1; --i)
				{
					b1.extend(buckets[i].bbox);
					count1 += buckets[i].count;

					auto count0 = node->elements - count1;
					auto sum = count0 * cumulative_buckets[i - 1].surface_area() + count1 * b1.surface_area();

					// Surface area cost weighted by number of elements in a bucket
					auto current_cost = .125f + sum / node->bbox.surface_area();

					if (cost > current_cost)
					{
						left_box = cumulative_buckets[i - 1];
						right_box = b1;
						cost = current_cost;
						right_count = count1;
						axis = extent;
					}
				}

				std::fill(buckets.begin(), buckets.end(), Bucket());
			}

			auto cut = primitives.begin() + node->end - right_count;
			auto begin = primitives.begin() + node->start;
			auto end = primitives.begin() + node->end;

			node->left = std::make_unique<Node>(left_box, node->start, node->end - right_count);
			node->right = std::make_unique<Node>(right_box, node->end - right_count, node->end);

			std::nth_element(begin, cut, end, [axis](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b){
				return a->get_bbox().centroid()[axis] < b->get_bbox().centroid()[axis];
			});

			middle_split(node->left.get());
			middle_split(node->right.get());
		}

		void equal_split(Node* node){}

		void initialize(){

			if (!root) root = std::make_unique<Node>(0, primitives.size());

			for (auto& primitive : primitives)
				root->bbox.extend(primitive->get_bbox());
		}
	};
}
