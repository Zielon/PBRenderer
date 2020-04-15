#include "projective.h"

pbr::ProjectiveCamera::ProjectiveCamera(glm::ivec2 film_size) : Camera(film_size){

	camera_to_screen = glm::perspective(glm::radians(fov), aspect, near, far);
}

pbr::Ray pbr::ProjectiveCamera::cast_ray(glm::vec2 viewport, glm::vec2 offset){

	const auto screen = (viewport + offset) / (get_film()->get_size() * 0.5f) - 1.f;
	const auto ndc = glm::vec4(screen.x, -screen.y, -1, 1);
	const auto unproject = inverse(camera_to_screen * lookAt(glm::vec3(0), coordinate.direction, coordinate.up));
	const glm::vec3 ray = unproject * ndc;

	return {coordinate.position, ray};
}

void pbr::ProjectiveCamera::update_shader(const std::shared_ptr<rasterizer::Shader>& shader) const{

	shader->setMat4("projection", camera_to_screen);
	shader->setMat4("view", world_to_camera());
	shader->setVec3("direction", coordinate.direction);
}

glm::mat4 pbr::ProjectiveCamera::world_to_camera() const{

	return lookAt(coordinate.position, coordinate.position + coordinate.direction, coordinate.up);
}

void pbr::ProjectiveCamera::set_fov(float fov){

	camera_to_screen = glm::perspective(glm::radians(fov), aspect, near, far);
	this->fov = fov;
}
