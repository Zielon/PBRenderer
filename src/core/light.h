#pragma once

#include "../geometry/scene_object.h"

namespace pbr
{
	class Light : SceneObject
	{
	public:
		bool intersect(const Ray& ray, Intersection& intersection) const override{

			return false;
		};

		BBox get_bbox() const override{

			return {};
		};

		void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe) override{};
	};
}
