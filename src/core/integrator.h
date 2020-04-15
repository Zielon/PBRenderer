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

		virtual void render() = 0;

		virtual std::shared_ptr<Film> get_film() const;

		virtual std::shared_ptr<Camera> get_camera() const;

	protected:
		std::shared_ptr<Scene> scene;
		std::shared_ptr<Film> film;
		std::shared_ptr<Camera> camera;
	};
}
