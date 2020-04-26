#pragma once

#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>

namespace pbr
{
	class Transformation
	{
	public:
		Transformation() = default;

		Transformation(glm::vec3 rotation_axis, float degrees, glm::vec3 scaling, glm::vec3 translation);

		glm::vec3 vector_to_world(glm::vec3 vector) const;

		glm::vec3 vector_to_local(glm::vec3 vector) const;

		glm::vec3 normal_to_world(glm::vec3 vector) const;

		float rotation_degree;
		glm::vec3 rotation_axis{};
		glm::vec3 scaling{};
		glm::vec3 translation{};
		glm::mat4 to_world_mat = glm::mat4(1.0f);
		glm::mat4 to_local_mat = glm::mat4(1.0f);
		glm::mat4 normal_to_world_mat = glm::mat4(1.0f);
	};
}
