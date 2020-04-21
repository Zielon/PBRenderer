#include "material.h"

#include "../materials/mirror.h"
#include "../materials/lambertian.h"

std::shared_ptr<pbr::Material> pbr::Material::create_material(parser::MaterialConfig& config){

	if (config.type == "LAMBERTIAN")
	{
		return std::dynamic_pointer_cast<Material, LambertianMaterial>(
			std::make_shared<LambertianMaterial>(config.textures.begin()->second));
	}

	if (config.type == "MIRROR")
	{
		return std::dynamic_pointer_cast<Material, MirrorMaterial>(
			std::make_shared<MirrorMaterial>(config.textures.begin()->second));
	}

	if (config.type == "GLASS") { }

	throw std::runtime_error("Wrong material type!");
}
