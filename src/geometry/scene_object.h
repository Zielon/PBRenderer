#pragma once

#include "../rasterizer/shader.h"

namespace pbr
{
	class Intersection;
	class Ray;

	class SceneObject
	{
	public:
		virtual ~SceneObject() = default;

		virtual bool intersect(const Ray& ray, Intersection& intersection) const = 0;

		virtual BBox get_bbox() const = 0;

		virtual void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe) = 0;

		int id{};

	private:
		BBox bbox;
	};
}
