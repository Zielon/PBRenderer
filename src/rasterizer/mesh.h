#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "shader.h"

namespace rasterizer
{
	struct Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	class Mesh
	{
	public:
		Mesh(
			std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures);

		void draw(const std::shared_ptr<Shader>& shader);

		std::vector<Vertex>& getVertices(){
			return vertices;
		}

		std::vector<unsigned int>& getIndices(){
			return indices;
		}

		std::vector<Texture>& getTextures(){
			return textures;
		}

	private:
		GLuint VAO{}, VBO{}, EBO{};

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		void setup();
	};
}
