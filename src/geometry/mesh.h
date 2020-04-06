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
	struct GL_Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	struct GL_Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	class Mesh : public pbr::SceneObject
	{
	public:
		Mesh(
			std::vector<GL_Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<GL_Texture> textures);

		void draw(const std::shared_ptr<rasterizer::Shader>& shader);

		bool intersect(const pbr::Ray& ray, pbr::Intersection& intersection) override;

		pbr::BBox getBBox() override;

	private:
		GLuint VAO{}, VBO{}, EBO{};

		pbr::BBox bbox;
		std::shared_ptr<pbr::BVH<pbr::Triangle>> bvh;

		std::vector<GL_Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<GL_Texture> textures;

		void generate_triangle();

		void generate_gl_buffers();
	};
}
