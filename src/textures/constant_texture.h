#pragma once

#include "texture.h"

namespace pbr
{
	class ConstantTexture : public Texture
	{
	public:

		ConstantTexture(glm::vec3 value): value(value){}

		glm::vec3 evaluate(Intersection& intersection) const override;

	private:
		glm::vec3 value{};
	};

	inline glm::vec3 ConstantTexture::evaluate(Intersection& intersection) const{

		return value;
	}
}
