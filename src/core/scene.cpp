#include "scene.h"
#include "../lights/infinite_light.h"

bool pbr::Scene::intersect(const Ray& ray, Intersection& intersection) const{

	return bvh->intersect(ray, intersection);
}

void pbr::Scene::draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe){

	for (auto& object : objects)
		object->draw(shader, wireframe);
}

void pbr::Scene::add_mesh(const std::shared_ptr<SceneObject>& mesh){

	bvh->add(mesh);
	objects.push_back(mesh);
}

void pbr::Scene::add_light(const std::shared_ptr<Light>& light){

	lights.push_back(light);
}

void pbr::Scene::add_environment_light(const std::shared_ptr<InfiniteLight>& light){

	environment_lights.push_back(light);
}

void pbr::Scene::build() const{

	bvh->build(Split::SAH);
}

std::shared_ptr<pbr::Camera> pbr::Scene::get_camera() const{

	return camera;
}

std::reference_wrapper<std::vector<std::shared_ptr<pbr::SceneObject>>> pbr::Scene::get_objects(){

	return std::ref(objects);
}

std::reference_wrapper<std::vector<std::shared_ptr<pbr::Light>>> pbr::Scene::get_lights(){

	return std::ref(lights);
}

std::reference_wrapper<std::vector<std::shared_ptr<pbr::InfiniteLight>>> pbr::Scene::get_environment_lights(){

	return std::ref(environment_lights);
}
