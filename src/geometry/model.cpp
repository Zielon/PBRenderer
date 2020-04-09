#include "model.h"

#include <iostream>
#include <direct.h>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

general::Model::Model(){
	reload_model(0);
}

void general::Model::load_model(const std::string& path){

	textures_loaded.clear();
	meshes.clear();

	Assimp::Importer importer;

	const aiScene* scene =
		importer.ReadFile(
			path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('\\'));
	process_node(scene->mRootNode, scene);
}

void general::Model::draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe){

	for (auto mesh : meshes)
	{
		mesh.draw(shader, wireframe);
	}
}

void general::Model::reload_model(int model){

	if (current_model == model)
		return;

	current_model = model;

	const std::string armadillo = R"(..\resoruces\armadillo\armadillo.ply)";
	const std::string nanosuit = R"(..\resoruces\nanosuit\nanosuit.obj)";
	const std::string cyborg = R"(..\resoruces\cyborg\cyborg.obj)";

	switch (current_model)
	{
	case 0:
		load_model(armadillo);
		break;
	case 1:
		load_model(nanosuit);
		break;
	case 2:
		load_model(cyborg);
		break;
	default:
		throw std::runtime_error("");
	}
}

void general::Model::process_node(aiNode* node, const aiScene* scene){

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		process_node(node->mChildren[i], scene);
}

general::Mesh general::Model::process_mesh(aiMesh* mesh, const aiScene* scene){

	std::vector<GL_Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<GL_Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		GL_Vertex vertex{};

		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = vec;
		}
		else
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);

		// To generate tangents UV coordinates are needed.
		if (mesh->mTangents)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		if (mesh->mBitangents)
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 1. diffuse maps
	std::vector<GL_Texture> diffuseMaps = load_material_textures(
		material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// 2. specular maps
	std::vector<GL_Texture> specularMaps = load_material_textures(
		material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3. normal maps
	std::vector<GL_Texture> normalMaps = load_material_textures(
		material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4. height maps
	std::vector<GL_Texture> heightMaps = load_material_textures(
		material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<general::GL_Texture> general::Model::load_material_textures(
	aiMaterial* mat, aiTextureType type, std::string type_name){

	std::vector<GL_Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;

		for (auto& j : textures_loaded)
		{
			if (std::strcmp(j.path.data(), str.C_Str()) == 0)
			{
				textures.push_back(j);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			GL_Texture texture;
			texture.id = texture_from_file(str.C_Str(), this->directory);
			texture.type = type_name;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int general::Model::texture_from_file(const char* path, const std::string& directory, bool gamma){

	std::string filename = std::string(path);
	filename = directory + "\\" + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format = 0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cerr << "ERROR::TEXTURE_FROM_FILE Failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
