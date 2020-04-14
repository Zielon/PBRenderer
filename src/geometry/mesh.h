#pragma once

#define GLM_FORCE_SWIZZLE 
#define GLM_SWIZZLE_XYZW 
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "../rasterizer/shader.h"

#include "../accelerators/bvh.h"
#include "triangle.h"
#include "scene_object.h"
#include "transformation.h"
#include "../parser/types.h"

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
		float mesh_id;
	};

	/**
	 * By default everything is calculated in the world space
	 */
	class Mesh : public pbr::SceneObject
	{
	public:
		Mesh(std::vector<GL_Vertex> vertices,
		     std::vector<unsigned int> indices,
		     std::vector<GL_Texture> textures,
			 parser::MeshConfig configuration);

		void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe) override;

		bool intersect(const pbr::Ray& ray, pbr::Intersection& intersection) const override;

		pbr::BBox get_bbox() const override;

		pbr::Transformation transformation;

	private:
		GLuint VAO{}, VBO{}, EBO{};

		std::string name;
		pbr::BBox bbox;
		std::shared_ptr<pbr::BVH<pbr::Triangle>> bvh;
		std::vector<GL_Vertex> vertices;
		std::vector<GL_Texture> textures;
		std::vector<GLuint> indices;
		parser::MeshConfig configuration;

		void generate_triangle();

		void generate_gl_buffers();
	};
}
