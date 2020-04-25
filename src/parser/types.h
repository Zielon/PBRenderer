#pragma once

#include <map>
#include <string>
#include <glm/vec3.hpp>
#include <rapidjson/document.h>
#include "parser.h"
#include "../textures/texture.h"
#include "../textures/constant_texture.h"
#include "../textures/image_texture.h"

namespace parser
{
	struct LightConfig final
	{
		LightConfig() = default;

		explicit LightConfig(const rapidjson::Value& node){

			id = node["id"].GetInt();
			name = node["name"].GetString();
			type = node["type"].GetString();
			position = node.HasMember("position") ? Parser::string_to_vec3(node["position"].GetString()) : glm::vec3();
			intensity = node["intensity"].GetFloat();
			mesh_id = node.HasMember("mesh_id") ? node["mesh_id"].GetInt() : -1;
		}

		int id;
		std::string name;
		std::string type;
		glm::vec3 position{};
		float intensity{};
		int mesh_id;
	};

	struct MaterialConfig final
	{
		MaterialConfig() = default;

		explicit MaterialConfig(const rapidjson::Value& node){

			type = node["type"].GetString();
			const rapidjson::Value& textures_map = node["textures"];

			assert(textures_map.IsArray());

			for (auto itr = textures_map.Begin(); itr != textures_map.End(); ++itr)
			{
				const rapidjson::Value& attribute = *itr;
				assert(attribute.IsObject());
				const std::string name = attribute["name"].GetString();
				const std::string type = attribute["type"].GetString();
				const bool is_float = attribute.HasMember("float") ? attribute["float"].GetBool() : false;

				if (type == "CONSTANT")
				{
					if (is_float)
					{
						auto const_value = attribute["value"].GetFloat();
						textures_float[name] = std::make_shared<pbr::ConstantTexture<float>>(name, const_value);
					}
					else
					{
						auto const_value = Parser::string_to_vec3(attribute["value"].GetString());
						textures_vec3[name] = std::make_shared<pbr::ConstantTexture<glm::vec3>>(name, const_value);
					}
				}

				if (type == "IMAGE")
				{
					auto path = attribute["path"].GetString();
					auto width = attribute["width"].GetInt();
					auto height = attribute["height"].GetInt();

					textures_vec3[name] = std::make_shared<pbr::ImageTexture>(name, width, height, path);
				}
			}
		}

		std::string type;
		std::map<std::string, std::shared_ptr<pbr::Texture<float>>> textures_float;
		std::map<std::string, std::shared_ptr<pbr::Texture<glm::vec3>>> textures_vec3;
	};

	struct MeshConfig final
	{
		MeshConfig() = default;

		explicit MeshConfig(const rapidjson::Value& node){

			id = node["id"].GetInt();
			name = node["name"].GetString();
			path = node["path"].GetString();
			scaling = Parser::string_to_vec3(node["scaling"].GetString());
			translation = Parser::string_to_vec3(node["translation"].GetString());
			rotation_axis = Parser::string_to_vec3(node["rotation_axis"].GetString());
			rotation_degree = node["rotation_degree"].GetFloat();
			material_config = MaterialConfig(node["material"]);
		}

		int id{};
		std::string name;
		std::string path;
		glm::vec3 scaling{};
		glm::vec3 translation{};
		glm::vec3 rotation_axis{};
		float rotation_degree{};
		MaterialConfig material_config;
	};
}
