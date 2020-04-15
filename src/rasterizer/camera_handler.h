#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/camera.h"
#include <memory>

namespace rasterizer
{
	enum class Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.10f;

	class CameraHandler final
	{
	public:
		void set_camera_handler(std::shared_ptr<pbr::Camera> camera){

			CameraHandler::camera = std::move(camera);

			yaw = -90.0f;
			pitch = 0.0f;
			movement_speed = SPEED;
			mouse_sensitivity = SENSITIVITY;

			update_camera();
		}

		void process_keyboard(Movement type, float deltaTime) const{

			const float velocity = movement_speed * deltaTime;
			auto& coordinate = camera->get_coordinate().get();

			if (type == Movement::FORWARD) coordinate.position += coordinate.direction * velocity;
			if (type == Movement::BACKWARD) coordinate.position -= coordinate.direction * velocity;
			if (type == Movement::LEFT) coordinate.position -= coordinate.right * velocity;
			if (type == Movement::RIGHT) coordinate.position += coordinate.right * velocity;
		}

		void process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch = true){

			xoffset *= mouse_sensitivity;
			yoffset *= mouse_sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (constrainPitch)
			{
				if (pitch > 89.0f) pitch = 89.0f;
				if (pitch < -89.0f) pitch = -89.0f;
			}

			update_camera();
		}

		static void process_mouse_scroll(float yoffset){

			if (camera->get_fov() >= 1.0f && camera->get_fov() <= 45.0f) camera->set_fov(camera->get_fov() - yoffset);
			if (camera->get_fov() <= 1.0f) camera->set_fov(1.0f);
			if (camera->get_fov() >= 45.0f) camera->set_fov(45.0f);
		}

	private:
		static std::shared_ptr<pbr::Camera> camera;

		float yaw{};
		float pitch{};
		float movement_speed{};
		float mouse_sensitivity{};

		void update_camera() const{

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			camera->get_coordinate().get().update(front);
		}
	};
}
