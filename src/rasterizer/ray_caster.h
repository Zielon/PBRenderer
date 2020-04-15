#pragma once

#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>

#include <memory>
#include <algorithm>

#include "../core/scene.h"
#include "../core/camera.h"
#include "../geometry/triangle.h"

namespace rasterizer
{
	class RayCaster
	{
	public:
		RayCaster(std::shared_ptr<pbr::Scene> scene, std::shared_ptr<pbr::Camera> camera):
			scene(std::move(scene)), camera(std::move(camera)){}

		void pick(const std::shared_ptr<Shader>& shader, bool picking) const;

		void ray_cast_frame();

	private:
		std::shared_ptr<pbr::Scene> scene;
		std::shared_ptr<pbr::Camera> camera;
		bool saving = false;
	};
}
