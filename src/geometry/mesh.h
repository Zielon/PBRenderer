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

		parser::MeshConfig get_config() const;

		GL_Vertex get_vertex(int id) const override;

	private:
		GLuint VAO{}, VBO{}, EBO{};

		std::string name;
		pbr::BBox bbox;
		std::shared_ptr<pbr::BVH<pbr::Triangle>> bvh;
		std::vector<GL_Vertex> vertices;
		std::vector<GL_Texture> textures;
		std::vector<GLuint> indices;
		const parser::MeshConfig configuration;

		void generate_triangle();

		void generate_gl_buffers();
	};
}
