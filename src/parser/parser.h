#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <glm/vec3.hpp>

namespace parser
{
	class Parser
	{
	public:
		static std::vector<std::string> split_string(const std::string& s, char delimiter){

			std::vector<std::string> tokens;
			std::string token;
			std::istringstream tokenStream(s);

			while (std::getline(tokenStream, token, delimiter))
				tokens.push_back(token);

			return tokens;
		}

		static glm::vec3 string_to_vec3(const std::string& s){

			auto vector = split_string(s, ' ');
			return glm::vec3(std::stof(vector[0]), std::stof(vector[1]), std::stof(vector[2]));
		}
	};
}
