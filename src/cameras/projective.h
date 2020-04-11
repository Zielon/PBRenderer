#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../core/camera.h"
#include <iostream>

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
		glm::mat4 screen_to_raster{};
		glm::mat4 raster_to_screen{};

		Ray cast_ray(glm::vec2 screen, glm::vec2 offset) override{

			// 3d Normalised Device Coordinates
			const float x = 2.0f * (screen.x + offset.x) / film_size.x - 1.0f;
			const float y = 1.0f - 2.0f * (screen.y + offset.y) / film_size.y;

			// 4d Homogeneous Clip Coordinates
			const glm::vec4 ray_clip = glm::vec4(x, y, -1.f, 1.f);

			// 4d Eye (Camera) Coordinates
			glm::vec4 ray_eye = inverse(camera_to_screen) * ray_clip;
			ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);

			// 4d World Coordinates
			const glm::vec4 ray_world = normalize(camera_to_world() * ray_eye);

			return {position, glm::vec3(ray_world.x, ray_world.y, ray_world.z)};
		};

		glm::mat4 camera_to_world() const{

			return inverse(world_to_camera());
		}

		glm::mat4 world_to_camera() const{

			return lookAt(position, position + direction, up);
		}

		glm::mat4 raster_to_camera() const{

			return inverse(camera_to_screen) * raster_to_screen;
		}

		void set_fov(float fovxy) override{

			camera_to_screen = glm::perspective(glm::radians(fovxy), aspect, near, far);
			fov = fovxy;
		}
	};
}
