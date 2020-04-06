#include "mesh.h"
#include <utility>

general::Mesh::Mesh(std::vector<GL_Vertex> vertices,
                    std::vector<unsigned> indices,
                    std::vector<GL_Texture> textures):
	bvh(std::make_shared<pbr::BVH<pbr::Triangle>>()),
	vertices(std::move(vertices)),
	textures(std::move(textures)),
	indices(std::move(indices)){

	generate_triangle();
	generate_gl_buffers();

	bvh->build(pbr::Split::MIDDLE);
}

void general::Mesh::draw(const std::shared_ptr<rasterizer::Shader>& shader){

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
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

bool general::Mesh::intersect(const pbr::Ray& ray, pbr::Intersection& intersection){

	return false;
}

pbr::BBox general::Mesh::getBBox(){

	return bbox;
}

void general::Mesh::generate_triangle(){

	for (int i = 0; i < indices.size(); i += 3)
	{
		auto v0 = vertices[indices[i]].position;
		auto v1 = vertices[indices[i + 1]].position;
		auto v2 = vertices[indices[i + 2]].position;

		const auto min = glm::min(glm::min(v1, v2), v0);
		const auto max = glm::max(glm::max(v1, v2), v0);

		bvh->add(pbr::Triangle(v0, v1, v2, normalize(cross(v1 - v0, v2 - v0)), min, max));
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
