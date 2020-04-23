#include "material.h"

#include "../materials/mirror.h"
#include "../materials/lambertian.h"
#include "../materials/glass.h"
#include "../materials/metal.h"
#include "../materials/plastic.h"

std::shared_ptr<pbr::Material> pbr::Material::create_material(parser::MaterialConfig& config){

	if (config.type == "LAMBERTIAN")
	{
		return std::make_shared<LambertianMaterial>(config.textures_vec3.begin()->second);
	}

	if (config.type == "MIRROR")
	{
		return std::make_shared<MirrorMaterial>(config.textures_vec3.begin()->second);
	}

	if (config.type == "METAL")
	{
		return std::make_shared<MetalMaterial>();
	}

	if (config.type == "PLASTIC")
	{
		return std::make_shared<PlasticMaterial>();
	}

	if (config.type == "GLASS")
	{
		auto kr = config.textures_vec3["kr"];
		auto kt = config.textures_vec3["kt"];
		auto u_roughness = config.textures_float["u_roughness"];
		auto v_roughness = config.textures_float["v_roughness"];
		auto index = config.textures_float["index"];

		return std::make_shared<GlassMaterial>(kr, kt, u_roughness, v_roughness, index);
	}

	throw std::runtime_error("Wrong material type!");
}
