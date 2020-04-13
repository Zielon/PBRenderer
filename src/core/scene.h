#pragma once

#include <memory>
#include <vector>

#include "../accelerators/bvh.h"
#include "../geometry/scene_object.h"

#include "integrator.h"
#include "light.h"
#include "../geometry/model_loader.h"

namespace pbr
{
	class Scene
	{
	public:
		bool intersect(const Ray& ray, Intersection& intersection) const;

	private:
		std::shared_ptr<general::Model_loader> model;
		std::shared_ptr<BVH<SceneObject>> bvh;
		std::shared_ptr<Integrator> integrator;
		std::vector<std::shared_ptr<Light>> lights;
	};
}
