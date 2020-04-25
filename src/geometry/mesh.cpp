#include "mesh.h"

#include <utility>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <random>

template <typename Iterator>
Iterator select_randomly(Iterator start, Iterator end){
	static std::random_device rd;
	static std::mt19937 gen(rd());
	const std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(gen));
	return start;
}

pbr::Mesh::Mesh(std::vector<GL_Vertex> vertices,
                std::vector<unsigned> indices,
                std::vector<GL_Texture> textures,
                parser::MeshConfig config):
	SceneObject(config.id, MESH),
	bvh(std::make_shared<BVH<Triangle>>()),
	gl_vertices(std::move(vertices)),
	gl_textures(std::move(textures)),
	indices(std::move(indices)),
	configuration(config){

	material = Material::create_material(config.material_config);
	transformation = Transformation(
		config.rotation_axis,
		config.rotation_degree,
		config.scaling,
		config.translation
	);

	generate_triangle();
	generate_gl_buffers();

	bvh->build(Split::SAH);
}

pbr::Sample pbr::Mesh::sample(const glm::vec2& u) const{

	std::vector<Sample> samples;

	for (auto& triangle : bvh->get_primitives())
		samples.emplace_back(triangle->sample(u));

	return *select_randomly(samples.begin(), samples.end());
}

void pbr::Mesh::draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe){

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < gl_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = gl_textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, gl_textures[i].id);
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

bool pbr::Mesh::intersect(const Ray& ray, Intersection& intersection) const{

	return bvh->intersect(ray, intersection);
}

pbr::BBox pbr::Mesh::get_bbox() const{

	return bbox;
}

std::shared_ptr<pbr::Material> pbr::Mesh::get_material() const{

	return material;
}

pbr::GL_Vertex pbr::Mesh::get_vertex(int id) const{

	return gl_vertices[id];
}

void pbr::Mesh::generate_triangle(){

	for (auto i = 0; i < indices.size(); i += 3)
	{
		auto a = gl_vertices[indices[i]].position;
		auto b = gl_vertices[indices[i + 1]].position;
		auto c = gl_vertices[indices[i + 2]].position;

		auto n = transformation.normal_to_world(normalize(cross(b - a, c - a)));
		auto v0 = transformation.vector_to_world(a);
		auto v1 = transformation.vector_to_world(b);
		auto v2 = transformation.vector_to_world(c);

		const auto min = glm::min(glm::min(v1, v2), v0);
		const auto max = glm::max(glm::max(v1, v2), v0);

		auto ids = glm::ivec3(indices[i], indices[i + 1], indices[i + 2]);
		auto triangle = std::make_shared<Triangle>(ids, v0, v1, v2, n, min, max, this);

		bvh->add(triangle);

		bbox.extend(min, max);
	}
}

void pbr::Mesh::generate_gl_buffers(){

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gl_vertices.size() * sizeof(GL_Vertex), &gl_vertices[0], GL_STATIC_DRAW);

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

	// vertex ids
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex),
	                      reinterpret_cast<void*>(offsetof(GL_Vertex, mesh_id)));

	glBindVertexArray(0);
}
