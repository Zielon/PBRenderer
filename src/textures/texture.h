#pragma once

#include <glm/vec3.hpp>
#include <utility>
#include <string>
#include <rapidjson/document.h>

namespace pbr
{
	class Intersection;

	template <typename T>
	class Texture
	{
	public:
		Texture<T>(std::string name): name(std::move(name)){}

		virtual ~Texture() = default;

		virtual T evaluate(Intersection& intersection) const = 0;

		std::string name;
	};
}
