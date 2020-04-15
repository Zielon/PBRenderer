#include "camera.h"

void pbr::Camera::set_fov(float fov){

	this->fov = fov;
}

float pbr::Camera::get_fov() const{

	return fov;
}

std::shared_ptr<pbr::Film> pbr::Camera::get_film() const{

	return film;
}

std::reference_wrapper<pbr::CoordinateSystem> pbr::Camera::get_coordinate(){

	return std::ref(coordinate);
}
