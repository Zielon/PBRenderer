#pragma once

#include <glm/vec3.hpp>
#include <memory>

#include "film.h"
#include "../geometry/ray.h"
#include "../rasterizer/shader.h"
#include "../geometry/camera_system.h"

namespace pbr
{
	class Camera
	{
	public:
		Camera(glm::vec2 film_size): film(std::make_shared<Film>(film_size)){

			aspect = film_size.x / film_size.y;
		}

		virtual ~Camera() = default;

		virtual glm::mat4 world_to_camera() const = 0;

		virtual void update_shader(const std::shared_ptr<rasterizer::Shader>& shader) const = 0;

		virtual Ray cast_ray(glm::vec2 screen, glm::vec2 offset) = 0;

		virtual void set_fov(float fov);

		virtual float get_fov() const;

		virtual std::shared_ptr<Film> get_film() const;

		std::reference_wrapper<CameraSystem> get_coordinate();

		bool is_active = true;

	protected:
		float fov = 45.f;
		float aspect;
		std::shared_ptr<Film> film;
		CameraSystem coordinate;
	};
}
