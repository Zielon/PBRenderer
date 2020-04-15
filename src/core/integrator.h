#pragma once

#include "scene.h"
#include "film.h"

namespace pbr
{
	class Integrator
	{
	public:
		Integrator(std::shared_ptr<Scene> scene):
			scene(std::move(scene)){}

		virtual ~Integrator() = default;

		virtual void render() = 0;

		virtual Film* get_film(){

			return scene->camera->film.get();
		}

		virtual general::Camera* get_camera(){

			return scene->camera.get();
		}

	protected:
		std::shared_ptr<Scene> scene;
		std::shared_ptr<Film> film;
		std::shared_ptr<general::Camera> camera;
	};
}
