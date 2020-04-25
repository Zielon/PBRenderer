#pragma once

#include "../rasterizer/shader.h"
#include "../geometry/bbox.h"
#include "../core/material.h"
#include "transformation.h"

namespace pbr
{
	struct GL_Texture
	{
		GL_Texture() = default;

		unsigned int id;
		std::string type;
		std::string path;
	};

	struct GL_Vertex
	{
		GL_Vertex() = default;

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		float mesh_id;

		GL_Vertex operator*(float s){

			normal *= s;
			tangent *= s;
			bitangent *= s;
			tex_coords *= s;
			return *this;
		};
	};

	class Intersection;
	class Ray;

	class SceneObject
	{
	public:
		SceneObject(int id): id(id){}

		virtual ~SceneObject() = default;

		virtual bool intersect(const Ray& ray, Intersection& intersection) const = 0;

		virtual BBox get_bbox() const = 0;

		virtual std::shared_ptr<Material> get_material() const = 0;

		virtual void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe) = 0;

		virtual GL_Vertex get_vertex(int id) const = 0;

		Transformation transformation;

		int id;

	protected:
		BBox bbox;
		std::shared_ptr<Material> material;
	};
}
