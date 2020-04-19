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
#include "../textures/texture.h"
#include "../core/material.h"
#include "../textures/image_texture.h"

namespace pbr
{
	/**
	 * By default everything is calculated in the world space
	 */
	class Mesh : public SceneObject
	{
	public:
		Mesh(std::vector<GL_Vertex> vertices,
		     std::vector<unsigned int> indices,
		     std::vector<GL_Texture> textures,
		     parser::MeshConfig config);

		void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe) override;

		bool intersect(const Ray& ray, Intersection& intersection) const override;

		BBox get_bbox() const override;

		parser::MeshConfig get_config() const;

		GL_Vertex get_vertex(int id) const override;

	private:
		GLuint VAO{}, VBO{}, EBO{};

		std::string name;
		BBox bbox;
		std::shared_ptr<BVH<Triangle>> bvh;
		std::vector<GL_Vertex> gl_vertices;
		std::vector<GL_Texture> gl_textures;
		std::vector<GLuint> indices;

		std::shared_ptr<Image> texture;

		const parser::MeshConfig configuration;

		void generate_triangle();

		void generate_gl_buffers();
	};
}
