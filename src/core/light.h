#pragma once

#include <utility>
#include <glm/vec2.hpp>

#include "../parser/types.h"

namespace pbr
{
	enum LightType{ AREA, POINT };

	class Light
	{
	public:
		virtual ~Light() = default;

		Light(LightType type, parser::LightConfig config): configuration(std::move(config)), type(type){}

		virtual glm::vec3 power() const = 0;

		virtual glm::vec3 sample_Li(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const = 0;

		virtual float pdf_Li(const Intersection &ref, const glm::vec3 &wi) const = 0;

		virtual float sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const = 0;

		//virtual void pdf_Le(const Ray &ray, const Normal3f &nLight, Float *pdfPos, Float *pdfDir) const = 0;

		LightType type;

	protected:
		const parser::LightConfig configuration;
	};
}
