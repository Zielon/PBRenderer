#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <rapidjson/document.h>
#include "parser.h"
#include "../textures/texture.h"
#include <map>

namespace parser
{
	struct LightConfig final
	{
		LightConfig() = default;

		explicit LightConfig(const rapidjson::Value& node){

			id = node["id"].GetInt();
			name = node["name"].GetString();
			type = node["type"].GetString();
			position = Parser::string_to_vec3(node["position"].GetString());
			intensity = node["intensity"].GetFloat();
			area_light = type == "AREA_LIGHT";
		}

		std::string id;
		std::string name;
		std::string type;
		glm::vec3 position{};
		float intensity{};
		bool area_light;
	};

	struct MaterialConfig final
	{
		MaterialConfig() = default;

		explicit MaterialConfig(const rapidjson::Value& node){

			type = node["type"].GetString();
			ior = node.HasMember("ior") ? node["ior"].GetFloat() : 0.f;
			
			const rapidjson::Value& t = node["textures"];

			assert(t.IsArray());

			for (auto itr = t.Begin(); itr != t.End(); ++itr)
			{
				const rapidjson::Value& attribute = *itr;
				assert(attribute.IsObject());
				const std::string t_type = attribute["type"].GetString();
				textures[t_type] = pbr::Texture::create(attribute);
			}
		}

		std::string type;
		float ior{};
		std::map<std::string, std::shared_ptr<pbr::Texture>> textures;
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
		}

		int id{};
		std::string name;
		std::string path;
		glm::vec3 scaling{};
		glm::vec3 translation{};
		glm::vec3 rotation_axis{};
		float rotation_degree{};
		MaterialConfig material{};
	};
}
