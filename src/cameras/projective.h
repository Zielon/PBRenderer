#pragma once

#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>

#undef near
#undef far

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../core/camera.h"

namespace pbr
{
	class ProjectiveCamera : public Camera
	{
	public:
		explicit ProjectiveCamera(glm::ivec2 film_size);

		ProjectiveCamera copy() const;

		Ray cast_ray(glm::vec2 viewport, glm::vec2 offset) override;

		void update_shader(const std::shared_ptr<rasterizer::Shader>& shader) const override;

		glm::mat4 world_to_camera() const override;

		void set_fov(float fov) override;

	private:
		float near = 0.01f;
		float far = 100.f;
		glm::mat4 camera_to_screen{};
	};
}
