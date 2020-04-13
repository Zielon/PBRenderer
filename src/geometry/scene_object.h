#pragma once

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

	private:
		BBox bbox;
	};
}
