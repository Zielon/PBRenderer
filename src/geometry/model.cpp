#include "model.h"

#include <iostream>
#include <direct.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

general::Model::Model(){

	loadModel();
}

void general::Model::loadModel(){

	Assimp::Importer importer;

	std::string path = R"(..\resoruces\nanosuit\nanosuit.obj)";

	const aiScene* scene =
		importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('\\'));
	process_node(scene->mRootNode, scene);
}

void general::Model::draw(const rasterizer::Shader& shader){

	for (auto mesh : meshes) mesh.draw(shader);
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

rasterizer::Mesh general::Model::process_mesh(aiMesh* mesh, const aiScene* scene){

	std::vector<rasterizer::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<rasterizer::Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		rasterizer::Vertex vertex{};

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

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;

		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector;

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
	std::vector<rasterizer::Texture> diffuseMaps = load_material_textures(
		material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// 2. specular maps
	std::vector<rasterizer::Texture> specularMaps = load_material_textures(
		material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3. normal maps
	std::vector<rasterizer::Texture> normalMaps = load_material_textures(
		material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4. height maps
	std::vector<rasterizer::Texture> heightMaps = load_material_textures(
		material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return rasterizer::Mesh(vertices, indices, textures);
}

std::vector<rasterizer::Texture> general::Model::load_material_textures(
	aiMaterial* mat, aiTextureType type, std::string type_name){

	std::vector<rasterizer::Texture> textures;
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
			rasterizer::Texture texture;
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
