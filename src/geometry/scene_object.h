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
	};
}
