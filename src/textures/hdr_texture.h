#pragma once

#include "texture.h"
#include <vector>
#include <glm/vec2.hpp>

namespace pbr
{
	class HDRTexture : public Texture<glm::vec3>
	{
	public:

		HDRTexture(std::string name, const std::string& path);

		glm::vec3 evaluate(Intersection& intersection) const override;

		glm::vec3 get(glm::vec2 uv);

		glm::vec2 get_size() const;

	private:
		int width{};
		int height{};
		std::vector<std::vector<glm::vec3>> pixels;
	};
}
