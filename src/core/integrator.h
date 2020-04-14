#pragma once

#include "scene.h"
#include "film.h"

namespace pbr
{
	class Integrator
	{
	public:
		Integrator(std::shared_ptr<Scene> scene, std::shared_ptr<Film> film):
			scene(std::move(scene)), film(std::move(film)){}

		virtual ~Integrator() = default;

		virtual void render() = 0;

	private:
		std::shared_ptr<Scene> scene;
		std::shared_ptr<Film> film;
	};
}
