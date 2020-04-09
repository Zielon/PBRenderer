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

	const float SPEED = 3.5f;
	const float SENSITIVITY = 0.10f;

	class Camera_handler final
	{
	public:

		void set_camera_handler(std::shared_ptr<general::Camera> camera){

			Camera_handler::camera = std::move(camera);

			yaw = 90.0f;
			pitch = 0.0f;
			movement_speed = SPEED;
			mouse_sensitivity = SENSITIVITY;

			update_camera();
		}

		void process_keyboard(Movement type, float deltaTime){

			const float velocity = movement_speed * deltaTime;

			if (type == Movement::FORWARD) camera->position += camera->direction * velocity;
			if (type == Movement::BACKWARD) camera->position -= camera->direction * velocity;
			if (type == Movement::LEFT) camera->position -= camera->right * velocity;
			if (type == Movement::RIGHT) camera->position += camera->right * velocity;
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

		void process_mouse_scroll(float yoffset){

			if (camera->fov >= 1.0f && camera->fov <= 45.0f) camera->fov -= yoffset;
			if (camera->fov <= 1.0f) camera->fov = 1.0f;
			if (camera->fov >= 45.0f) camera->fov = 45.0f;
		}

	private:

		static std::shared_ptr<general::Camera> camera;

		float yaw{};
		float pitch{};

		float movement_speed{};
		float mouse_sensitivity{};

		void update_camera() const{

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			// Camera coordinate system
			camera->direction = normalize(front);
			camera->right = normalize(cross(camera->direction, glm::vec3(0.0f, 1.0f, 0.0f)));
			camera->up = normalize(cross(camera->right, camera->direction));
		}
	};
}
