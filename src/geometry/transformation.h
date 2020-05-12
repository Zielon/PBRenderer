#pragma once

#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>

namespace pbr
{
	struct GL_Vertex;

	class Transformation
	{
	public:
		Transformation() = default;

		Transformation(glm::vec3 rotation_euler, glm::vec3 scaling, glm::vec3 translation);

		glm::vec3 vector_to_world(glm::vec3 vector) const;

		glm::vec3 vector_to_local(glm::vec3 vector) const;

		glm::vec3 normal_to_world(glm::vec3 vector) const;

		GL_Vertex vertex_to_world(const GL_Vertex& vertex) const;

		glm::vec3 rotation{0.f};
		glm::vec3 scaling{0.f};
		glm::vec3 translation{0.f};

		glm::mat4 to_world_mat = glm::mat4(1.0f);
		glm::mat4 to_local_mat = glm::mat4(1.0f);
		glm::mat4 normal_to_world_mat = glm::mat4(1.0f);
	};
}
