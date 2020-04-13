#include "transformation.h"
#include <glm/ext/matrix_transform.inl>

pbr::Transformation::Transformation(glm::vec3 rotation_axis, float degrees, glm::vec3 scaling, glm::vec3 translation) :
	rotation_degree(degrees), rotation_axis(rotation_axis), scaling(scaling), translation(translation){

	to_world = translate(to_world, translation);
	to_world = rotate(to_world, glm::radians(rotation_degree), rotation_axis);
	to_world = scale(to_world, scaling);

	to_local = inverse(to_world);
}

glm::vec3 pbr::Transformation::vector_to_world(glm::vec3 vector) const{

	return to_world * glm::vec4(vector.xyz(), 1.f);
}

glm::vec3 pbr::Transformation::vector_to_local(glm::vec3 vector) const{

	return to_local * glm::vec4(vector.xyz(), 1.f);
}