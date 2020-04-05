#pragma once

#include <memory>
#include <vector>

#include "integrator.h"
#include "light.h"
#include "../accelerators/bvh.h"
#include "../geometry/scene_object.h"

namespace pbr
{
	class Scene
	{
	public:

	private:
		BVH<std::shared_ptr<general::SceneObject>> bvh;
		std::shared_ptr<Integrator> integrator;
		std::vector<std::shared_ptr<Light>> lights;
	};
}
