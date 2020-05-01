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
			Light(AREA, config), mesh_id(config.mesh_id), position(config.position), luminous(config.intensity),
			two_sided(true), scene_light(std::move(mesh)){}

		glm::vec3 sample_Li(
			const Intersection& intersection, const std::shared_ptr<Scene>& scene, const glm::vec2& u, glm::vec3* wi,
			float* pdf, bool* shadow) const override;

		float pdf_Li(const Intersection& ref, const std::shared_ptr<Scene>& scene, const glm::vec3& wi) const override;

		float sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const override;

		glm::vec3 power() const override;

		glm::vec3 L(const glm::vec3& n, const glm::vec3& w) const;

	private:
		int mesh_id;
		const glm::vec3 position;
		const glm::vec3 luminous;
		bool two_sided;

		/*
		 * Area light is represented in the rasterizer as this mesh.
		 */
		std::shared_ptr<SceneObject> scene_light;
	};
}
