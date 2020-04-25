#pragma once

#include <utility>

#include "../geometry/scene_object.h"
#include "../core/light.h"

namespace pbr
{
	class AreaLight : public Light
	{
	public:
		AreaLight(const parser::LightConfig& config, std::shared_ptr<SceneObject> mesh):
			Light(AREA, config), mesh_id(config.mesh_id), mesh(std::move(mesh)){}

		glm::vec3 sample_Li(
			const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const override;

		float pdf_Li(const Intersection& ref, const glm::vec3& wi) const override;

		float sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const override;

		glm::vec3 power() const override;

	private:
		int mesh_id;

		/*
		 * Area light is represented in the rasterizer as this mesh.
		 */
		std::shared_ptr<SceneObject> mesh;
	};
}
