#pragma once

#include <utility>
#include <glm/vec2.hpp>

#include "../parser/types.h"

namespace pbr
{
	enum LightType { AREA, POINT, INFINITE };

	class Ray;
	class Scene;
	class Intersection;

	class Light
	{
	public:
		virtual ~Light() = default;

		Light(LightType type, parser::LightConfig config) : type(type), configuration(std::move(config)) {}

		virtual glm::vec3 power() const = 0;

		virtual glm::vec3 sample_Li(
			const Intersection& intersection,
			const std::shared_ptr<Scene>& scene,
			const glm::vec2& u,
			glm::vec3* wi,
			float* pdf,
			bool* shadow) const = 0;

		virtual float pdf_Li(
			const Intersection& ref,
			const std::shared_ptr<Scene>& scene,
			const glm::vec3& wi) const = 0;

		virtual float sample_Le(
			const Intersection& intersection,
			const glm::vec2& u,
			glm::vec3* wi,
			float* pdf) const = 0;

		virtual glm::vec3 Le(const Ray& ray) const {

			return glm::vec3(0.f);
		}

		LightType type;

	protected:
		const parser::LightConfig configuration;
		float ray_epsilon = 0.00001f;
	};
}
