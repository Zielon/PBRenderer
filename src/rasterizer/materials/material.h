#pragma once

#include <glm/vec3.hpp>
#include <map>

namespace rasterizer
{
	class Material
	{
	public:
		Material();

		~Material();

	private:
		std::map<std::string, glm::vec3> values;
	};
}
