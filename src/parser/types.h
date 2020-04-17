#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <rapidjson/document.h>
#include "parser.h"

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

			id = node["id"].GetInt();
			type = node["type"].GetString();
			ior = node["ior"].GetFloat();
			kd = node.HasMember("kd") ? node["kd"].GetFloat() : 0.f;
			ks = node.HasMember("ks") ? node["ks"].GetFloat() : 0.f;
			specular = node.HasMember("specular") ? node["specular"].GetFloat() : 0.f;
		}

		int id{};
		std::string type;
		float ior{};
		float kd{};
		float ks{};
		float specular{};
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
			color = Parser::string_to_vec3(node["color"].GetString());
			material_id = node["material_id"].GetInt();
		}

		int id{};
		std::string name;
		std::string path;
		glm::vec3 scaling{};
		glm::vec3 translation{};
		glm::vec3 rotation_axis{};
		float rotation_degree{};
		glm::vec3 color{};
		int material_id{};
		MaterialConfig material{};
	};
}
