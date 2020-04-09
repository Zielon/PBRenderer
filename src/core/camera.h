#pragma once

#include <glm/vec3.hpp>

namespace general
{
	class Camera
	{
	public:
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right{};
		float fov = 45.f;

		glm::mat4 get_view_matrix() const{

			return lookAt(position, position + direction, up);
		}
	};
}
