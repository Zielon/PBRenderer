#include "transformation.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "scene_object.h"

pbr::Transformation::Transformation(glm::vec3 rotation, glm::vec3 scaling, glm::vec3 translation) :
	rotation(rotation), scaling(scaling), translation(translation){

	to_world_mat = translate(to_world_mat, translation);
	to_world_mat = orientate4(radians(rotation)) * to_world_mat;
	to_world_mat = scale(to_world_mat, scaling);

	to_local_mat = inverse(to_world_mat);
	normal_to_world_mat = transpose(inverse(to_world_mat));
}

glm::vec3 pbr::Transformation::vector_to_world(glm::vec3 vector) const{

	return to_world_mat * glm::vec4(vector.xyz(), 1.f);
}

pbr::GL_Vertex pbr::Transformation::vertex_to_world(const GL_Vertex& vertex) const{

	GL_Vertex world{};

	world.position = vector_to_world(vertex.position);
	world.normal = normal_to_world(vertex.normal);
	world.bitangent = vector_to_world(vertex.bitangent);
	world.tangent = vector_to_world(vertex.tangent);
	world.mesh_id = vertex.mesh_id;
	world.tex_coords = vertex.tex_coords;

	return world;
}

glm::vec3 pbr::Transformation::vector_to_local(glm::vec3 vector) const{

	return to_local_mat * glm::vec4(vector.xyz(), 1.f);
}

glm::vec3 pbr::Transformation::normal_to_world(glm::vec3 vector) const{

	return normalize(normal_to_world_mat * glm::vec4(vector.xyz(), 1.f));
}
