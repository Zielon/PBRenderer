#pragma once

#include <memory>

#include "../core/material.h"
#include "../textures/texture.h"

namespace pbr
{
	class MirrorMaterial : public Material
	{
	public:
		MirrorMaterial(std::shared_ptr<Texture<glm::vec3>> kr) : kr(std::move(kr)){}

		void compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const override;

	private:
		std::shared_ptr<Texture<glm::vec3>> kr;
	};
}
