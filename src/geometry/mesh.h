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
#include "../parser/types.h"
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

		std::shared_ptr<Material> get_material() const override;

		float get_area() const override;

		Sample sample(const glm::vec2& u) const;

		GL_Vertex get_vertex(int id) const override;

		std::shared_ptr<AreaLight> get_area_light() const;

		void set_area_light(const std::shared_ptr<AreaLight>& light);

	private:
		GLuint VAO{}, VBO{}, EBO{};

		float area = 0;
		std::string name;
		BBox bbox;
		std::shared_ptr<BVH<Triangle>> bvh;
		std::vector<GL_Vertex> GL_vertices;
		std::vector<GL_Texture> GL_textures;
		std::vector<GLuint> indices;
		const parser::MeshConfig configuration;
		std::vector<GL_Vertex> vertices;
		std::shared_ptr<AreaLight> area_light{nullptr};

		void generate_triangle();

		void generate_gl_buffers();
	};
}
