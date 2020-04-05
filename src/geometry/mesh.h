#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "../rasterizer/shader.h"
#include "scene_object.h"
#include "../accelerators/bvh.h"
#include "triangle.h"

namespace general
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

	class Mesh : public SceneObject
	{
	public:
		Mesh(
			std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures);

		void draw(const std::shared_ptr<rasterizer::Shader>& shader);

		std::vector<Vertex>& get_vertices();

		std::vector<unsigned int>& get_indices();

		std::vector<Texture>& get_textures();

	private:
		GLuint VAO{}, VBO{}, EBO{};

		std::shared_ptr<pbr::BVH<pbr::Triangle>> bvh;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		void setup();
	};
}
