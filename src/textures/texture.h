#pragma once

#include <glm/vec3.hpp>
#include <rapidjson/document.h>

namespace pbr
{
	class Intersection;

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual glm::vec3 evaluate(Intersection& intersection) const = 0;

		static std::shared_ptr<Texture> create(const rapidjson::Value& node);
	};
}
