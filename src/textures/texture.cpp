#include "texture.h"
#include "constant_texture.h"
#include "../parser/parser.h"
#include "image_texture.h"

std::shared_ptr<pbr::Texture> pbr::Texture::create(const rapidjson::Value& node){

	const std::string type = node["type"].GetString();

	if (type == "CONSTANT")
	{
		auto const_value = parser::Parser::string_to_vec3(node["value"].GetString());

		return std::make_shared<ConstantTexture>(const_value);
	}

	if (type == "IMAGE")
	{
		auto path = node["path"].GetString();
		auto width = node["width"].GetInt();
		auto height = node["height"].GetInt();

		return std::make_shared<ImageTexture>(width, height, path);
	}

	return {};
}
