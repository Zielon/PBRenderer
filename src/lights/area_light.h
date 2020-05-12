#pragma once

#include <utility>

#include "../core/light.h"
#include "../geometry/scene_object.h"

namespace pbr
{
	class AreaLight : public Light
	{
	public:
		AreaLight(const parser::LightConfig& config, std::shared_ptr<SceneObject> mesh):
			Light(AREA, config), mesh_id(config.mesh_id), two_sided(false), position(config.position),
			luminous(config.intensity), scene_light(std::move(mesh)){}

		glm::vec3 sample_Li(
			const Intersection& intersection, const std::shared_ptr<Scene>& scene, const glm::vec2& u, glm::vec3* wi,
			float* pdf, bool* shadow) const override;

		float pdf_Li(const Intersection& ref, const std::shared_ptr<Scene>& scene, const glm::vec3& wi) const override;

		float sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const override;

		glm::vec3 power() const override;

		glm::vec3 L(const glm::vec3& n, const glm::vec3& w) const;

	private:
		int mesh_id;
		bool two_sided;
		const glm::vec3 position;
		const glm::vec3 luminous;

		/*
		 * Area light is represented in the rasterizer as this mesh.
		 */
		std::shared_ptr<SceneObject> scene_light;
	};
}
