#include "scene.h"

bool pbr::Scene::intersect(const Ray& ray, Intersection& intersection) const{

	return bvh->intersect(ray, intersection);
}

void pbr::Scene::draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe){

	for (auto& object : objects)
		object->draw(shader, wireframe);
}

void pbr::Scene::add_object(const std::shared_ptr<SceneObject>& mesh){

	bvh->add(mesh);
	objects.push_back(mesh);
}

void pbr::Scene::add_light(const std::shared_ptr<Light>& light){

	lights.push_back(light);
}

void pbr::Scene::build() const{

	bvh->build(Split::SAH);
}
