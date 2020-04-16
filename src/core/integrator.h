#pragma once

#include "scene.h"
#include "film.h"

namespace pbr
{
	class Integrator
	{
	public:
		Integrator(std::shared_ptr<Scene> scene): scene(std::move(scene)){}

		virtual ~Integrator() = default;

		virtual void render();

	protected:
		virtual glm::vec3 process(const Ray& ray, int depth) const = 0;

		virtual std::shared_ptr<Film> get_film() const;

		virtual std::shared_ptr<Camera> get_camera() const;

		int samples = 4;
		std::shared_ptr<Scene> scene;
	};
}
