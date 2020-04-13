#include "model_loader.h"

#include <iostream>
#include <direct.h>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <sstream>

void general::ModelLoader::load_models(const std::string& config){

	textures_loaded.clear();

	std::string name;

	std::ifstream ifs(config);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;

	document.ParseStream(isw);

	const rapidjson::Value& meshes = document["meshes"];
	assert(meshes.IsArray());

	for (rapidjson::Value::ConstValueIterator itr = meshes.Begin(); itr != meshes.End(); ++itr)
	{
		const rapidjson::Value& attribute = *itr;
		assert(attribute.IsObject());

		std::string path = attribute["path"].GetString();
		name = attribute["name"].GetString();

		auto mesh_id = attribute["id"].GetInt();
		auto translation = string_to_vec3(attribute["translation"].GetString());
		auto scaling = string_to_vec3(attribute["scaling"].GetString());
		auto rotation_axis = string_to_vec3(attribute["rotation_axis"].GetString());
		auto rotation_degree = attribute["rotation_degree"].GetFloat();

		pbr::Transformation transformation(rotation_axis, rotation_degree, scaling, translation);

		Assimp::Importer importer;

		auto flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
		const aiScene* ai_scene = importer.ReadFile(path, flags);

		if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP " << importer.GetErrorString() << std::endl;
			continue;
		}

		directory = path.substr(0, path.find_last_of('\\'));
		process_node(ai_scene->mRootNode, ai_scene, transformation, mesh_id);

		std::cout << "INFO::MESH [" << name << "] LOADED" << std::endl;
	}
}

void general::ModelLoader::process_node(
	aiNode* node, const aiScene* ai_scene, pbr::Transformation& transformation, int mesh_id){

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		auto mesh = process_mesh(ai_scene->mMeshes[node->mMeshes[i]], ai_scene, transformation, mesh_id);
		mesh->id = mesh_id;
		scene->add_object(mesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		process_node(node->mChildren[i], ai_scene, transformation, mesh_id);
}

std::shared_ptr<general::Mesh> general::ModelLoader::process_mesh(
	aiMesh* mesh, const aiScene* scene, pbr::Transformation& transformation, int mesh_id){

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

		vertex.mesh_id = mesh_id;

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

	return std::make_shared<Mesh>(vertices, indices, textures, transformation, mesh_id);
}

std::vector<general::GL_Texture> general::ModelLoader::load_material_textures(
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

std::vector<std::string> general::ModelLoader::split_string(const std::string& s, char delimiter) const{

	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);

	while (std::getline(tokenStream, token, delimiter))
		tokens.push_back(token);

	return tokens;
}

glm::vec3 general::ModelLoader::string_to_vec3(const std::string& s) const{

	auto parts = split_string(s, ' ');
	return glm::vec3(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
}

unsigned int general::ModelLoader::texture_from_file(const char* path, const std::string& directory, bool gamma){

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
