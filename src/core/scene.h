#pragma once

#include <memory>
#include <vector>

#include "light.h"
#include "../accelerators/bvh.h"
#include "../geometry/scene_object.h"
#include "../rasterizer/shader.h"

namespace pbr
{
	class Scene
	{
	public:
		Scene(): bvh(std::make_shared<BVH<SceneObject>>()){};

		bool intersect(const Ray& ray, Intersection& intersection) const;

		void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe);

		void add_object(const std::shared_ptr<SceneObject>& object);

		void build() const;

	private:
		std::shared_ptr<BVH<SceneObject>> bvh;

		std::vector<std::shared_ptr<Light>> lights;
		std::vector<std::shared_ptr<SceneObject>> objects;
	};
}
