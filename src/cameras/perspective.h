#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/camera.h"

namespace pbr
{
	class PerspectiveCamera : public general::Camera
	{
	public:
		explicit PerspectiveCamera(glm::ivec2 film_size): Camera(film_size){

			camera_to_screen = glm::perspectiveRH_NO(glm::radians(fov), aspect, near, far);
			screen_to_raster = glm::mat4(
				film_size.x / 2, 0, 0, film_size.x / 2,
				0, film_size.y / 2, 0, film_size.y / 2,
				0, 0, (far - near) / 2, (near + far) / 2,
				0, 0, 0, 1);
			raster_to_screen = inverse(screen_to_raster);
		}

		float near = 0.1f;
		float far = 100.f;

		glm::mat4 camera_to_screen{};
		glm::mat4 screen_to_raster{};
		glm::mat4 raster_to_screen{};

		glm::mat4 camera_to_world() const{

			return inverse(world_to_camera());
		}

		glm::mat4 raster_to_camera() const{

			return inverse(camera_to_screen) * raster_to_screen;
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
