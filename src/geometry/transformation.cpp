#include "transformation.h"
#include <glm/ext/matrix_transform.inl>

pbr::Transformation::Transformation(glm::vec3 rotation_axis, float degrees, glm::vec3 scaling, glm::vec3 translation) :
	rotation_degree(degrees), rotation_axis(rotation_axis), scaling(scaling), translation(translation){

	to_world_mat = translate(to_world_mat, translation);
	to_world_mat = rotate(to_world_mat, glm::radians(rotation_degree), rotation_axis);
	to_world_mat = scale(to_world_mat, scaling);

	to_local_mat = inverse(to_world_mat);
	normal_to_world_mat = transpose(inverse(to_world_mat));
}

glm::vec3 pbr::Transformation::vector_to_world(glm::vec3 vector) const{

	return to_world_mat * glm::vec4(vector.xyz(), 1.f);
}

glm::vec3 pbr::Transformation::vector_to_local(glm::vec3 vector) const{

	return to_local_mat * glm::vec4(vector.xyz(), 1.f);
}

glm::vec3 pbr::Transformation::normal_to_world(glm::vec3 vector) const{

	return normalize(normal_to_world_mat * glm::vec4(vector.xyz(), 1.f));
}
