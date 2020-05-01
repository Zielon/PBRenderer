#pragma once

#include <glm/glm.hpp>

namespace pbr
{
	/**
	 * Camera local coordinate system
	 */
	class CameraSystem
	{
	public:
		CameraSystem() = default;

		void update(glm::vec3 front){
			direction = normalize(front);
			right = normalize(cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
			up = normalize(cross(right, direction));
		}

		glm::vec3 position = glm::vec3(-0.25f, 0.2f, 1.4f);
		glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 right{};
	};
}
