#pragma once

#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../core/camera.h"

namespace pbr
{
	class ProjectiveCamera : public general::Camera
	{
	public:
		explicit ProjectiveCamera(glm::ivec2 film_size): Camera(film_size){

			camera_to_screen = glm::perspective(glm::radians(fov), aspect, near, far);
		}

		float near = 0.1f;
		float far = 100.f;

		glm::mat4 camera_to_screen{};

		Ray cast_ray(glm::vec2 screen, glm::vec2 offset) override{

			const float x = screen.x / (film_size.x * 0.5f) - 1.0f;
			const float y = screen.y / (film_size.y * 0.5f) - 1.0f;

			const auto ndc = glm::vec4(x, -y, -1, 1);
			const auto unproject = inverse(camera_to_screen * lookAt(glm::vec3(0), direction, up));
			const glm::vec3 ray = unproject * ndc;

			return {position, ray};
		}

		glm::mat4 world_to_camera() const{

			return lookAt(position, position + direction, up);
		}

		void set_fov(float fovxy) override{

			camera_to_screen = glm::perspective(glm::radians(fovxy), aspect, near, far);
			fov = fovxy;
		}
	};
}
