#pragma once

#include <memory>
#include <vector>

#include "light.h"
#include "camera.h"
#include "../accelerators/bvh.h"
#include "../geometry/scene_object.h"
#include "../rasterizer/shader.h"

namespace pbr
{
	class Scene
	{
	public:
		Scene(std::shared_ptr<Camera> camera): bvh(std::make_shared<BVH<SceneObject>>()),
		                                       camera(std::move(camera)){};

		bool intersect(const Ray& ray, Intersection& intersection) const;

		void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe);

		void add_mesh(const std::shared_ptr<SceneObject>& mesh);

		void add_light(const std::shared_ptr<Light>& light);

		void build() const;

		std::shared_ptr<Camera> get_camera() const;

		std::reference_wrapper<std::vector<std::shared_ptr<SceneObject>>> get_objects();

		std::reference_wrapper<std::vector<std::shared_ptr<Light>>> get_lights();

	private:
		std::shared_ptr<BVH<SceneObject>> bvh;
		std::shared_ptr<Camera> camera;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<std::shared_ptr<SceneObject>> objects;
	};
}
