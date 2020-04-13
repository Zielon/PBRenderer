#include "shader_manager.h"

rasterizer::ShaderManager::ShaderManager(){

	shaders["FLAT"] = std::make_shared<Shader>(R"(flat\flat.vert)", R"(flat\flat.frag)", R"(flat\flat.geo)");
	shaders["NORMALS"] = std::make_shared<Shader>(R"(flat\flat.vert)", R"(normals\normals.frag)", R"(flat\flat.geo)");
}

std::shared_ptr<rasterizer::Shader> rasterizer::ShaderManager::reload(int index){

	std::string type;

	switch (index)
	{
	case 0:
		type = "FLAT";
		break;
	case 1:
		type = "NORMALS";
		break;
	default:
		throw std::runtime_error("");
	}

	if (shaders.find(type) == shaders.end())
		throw std::runtime_error("");

	shaders[type]->use();

	return shaders[type];
}
