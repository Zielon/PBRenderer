#include "mesh.h"

#include <utility>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

general::Mesh::Mesh(std::vector<GL_Vertex> vertices,
                    std::vector<unsigned> indices,
                    std::vector<GL_Texture> textures):
	transformation(glm::vec3(0, 1, 0), 180.f, glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.14f, 0.15f, 0.f)),
	bvh(std::make_shared<pbr::BVH<pbr::Triangle>>()),
	vertices(std::move(vertices)),
	textures(std::move(textures)),
	indices(std::move(indices)){

	generate_triangle();
	generate_gl_buffers();

	bvh->build(pbr::Split::SAH);

	std::cout << "INFO::MESH INITIALIZED" << std::endl;
}

void general::Mesh::draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe){

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);

	shader->setMat4("model", transformation.to_world);
	shader->setVec3("color", glm::vec3(0.75f, 0.75f, 0.75f));

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

	if (wireframe)
	{
		shader->setVec3("color", glm::vec3(.0f, .0f, .0f));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

bool general::Mesh::intersect(const pbr::Ray& ray, pbr::Intersection& intersection) const{

	return bvh->intersect(ray, intersection);
}

pbr::BBox general::Mesh::get_bbox() const{

	return bbox;
}

void general::Mesh::generate_triangle(){

	for (auto i = 0; i < indices.size(); i += 3)
	{
		auto v0 = transformation.to_world * glm::vec4(vertices[indices[i]].position.xyz(), 1.f);
		auto v1 = transformation.to_world * glm::vec4(vertices[indices[i + 1]].position.xyz(), 1.f);
		auto v2 = transformation.to_world * glm::vec4(vertices[indices[i + 2]].position.xyz(), 1.f);

		const auto min = glm::min(glm::min(v1, v2), v0);
		const auto max = glm::max(glm::max(v1, v2), v0);

		auto ids = glm::ivec3(indices[i], indices[i + 1], indices[i + 2]);

		bvh->add(std::make_shared<pbr::Triangle>(ids, v0, v1, v2, min, max));

		bbox.extend(min, max);
	}
}

void general::Mesh::generate_gl_buffers(){

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GL_Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex), static_cast<void*>(nullptr));

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex),
	                      reinterpret_cast<void*>(offsetof(GL_Vertex, normal)));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex),
	                      reinterpret_cast<void*>(offsetof(GL_Vertex, tex_coords)));

	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex),
	                      reinterpret_cast<void*>(offsetof(GL_Vertex, tangent)));

	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex),
	                      reinterpret_cast<void*>(offsetof(GL_Vertex, bitangent)));

	glBindVertexArray(0);
}
