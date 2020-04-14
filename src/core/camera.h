#pragma once

#include <glm/vec3.hpp>
#include <memory>

#include "../geometry/ray.h"
#include "../rasterizer/shader.h"

namespace general
{
	class Camera
	{
	public:
		Camera(glm::vec2 film_size): film_size(film_size){

			aspect = film_size.x / film_size.y;
		}

		virtual ~Camera() = default;

		virtual glm::mat4 world_to_camera() const = 0;

		virtual void update_shader(const std::shared_ptr<rasterizer::Shader>& shader) const = 0;

		virtual pbr::Ray cast_ray(glm::vec2 screen, glm::vec2 offset){

			return {};
		}

		virtual void set_fov(float fovxy){

			fov = fovxy;
		}

		virtual float get_fov(){

			return fov;
		}

		float fov = 45.f;
		float aspect;

		glm::vec2 film_size;
		glm::vec3 position = glm::vec3(-0.25f, 0.2f, 1.f);

		// Camera coordinate system
		glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 right{};
	};
}
