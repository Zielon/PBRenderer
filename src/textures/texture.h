#pragma once

#include <rapidjson/document.h>

#include "../geometry/intersection.h"

namespace pbr
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual glm::vec3 evaluate(Intersection& intersection) const = 0;

		static std::shared_ptr<Texture> create(const rapidjson::Value& node);
	};
}
