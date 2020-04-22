#pragma once

#include <memory>
#include <utility>

#include "../core/material.h"
#include "../textures/texture.h"

namespace pbr
{
	class GlassMaterial : public Material
	{
	public:
		GlassMaterial(std::shared_ptr<Texture<glm::vec3>> kr,
		              std::shared_ptr<Texture<glm::vec3>> kt,
		              std::shared_ptr<Texture<float>> u_roughness,
		              std::shared_ptr<Texture<float>> v_roughness,
		              std::shared_ptr<Texture<float>> index):
			kr(std::move(kr)),
			kt(std::move(kt)),
			u_roughness(std::move(u_roughness)),
			v_roughness(std::move(v_roughness)),
			index(std::move(index)){}

		void compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const override;

	private:
		std::shared_ptr<Texture<glm::vec3>> kr;
		std::shared_ptr<Texture<glm::vec3>> kt;
		std::shared_ptr<Texture<float>> u_roughness;
		std::shared_ptr<Texture<float>> v_roughness;
		std::shared_ptr<Texture<float>> index;
	};
}
