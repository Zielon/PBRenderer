#include "loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <direct.h>
#include <glm/gtc/matrix_transform.hpp>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <assimp/postprocess.h>

#include "../parser/types.h"
#include "../lights/point_light.h"
#include "../lights/area_light.h"
#include "../lights/infinite_light.h"

void pbr::Loader::load_meshes(const std::string& config, Scene* scene){

	std::ifstream ifs(config);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;

	document.ParseStream(isw);

	const rapidjson::Value& meshes = document["meshes"];
	assert(meshes.IsArray());

	for (auto itr = meshes.Begin(); itr != meshes.End(); ++itr)
	{
		const rapidjson::Value& attribute = *itr;
		assert(attribute.IsObject());
		parser::MeshConfig configuration(attribute);
		Assimp::Importer importer;

		auto flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals;
		const aiScene* ai_scene = importer.ReadFile(configuration.path, flags);

		if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP " << importer.GetErrorString() << std::endl;
			continue;
		}

		directory = configuration.path.substr(0, configuration.path.find_last_of('\\'));
		process_node(ai_scene->mRootNode, ai_scene, configuration, scene);

		std::cout << "INFO::LOADER Mesh loaded -> [" << configuration.name << "]" << std::endl;
	}
}

void pbr::Loader::load_lights(const std::string& config, Scene* scene){

	std::ifstream ifs(config);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;

	document.ParseStream(isw);

	const rapidjson::Value& lights = document["lights"];
	assert(lights.IsArray());

	for (auto itr = lights.Begin(); itr != lights.End(); ++itr)
	{
		const rapidjson::Value& attribute = *itr;
		assert(attribute.IsObject());
		parser::LightConfig configuration(attribute);

		if (configuration.type == "POINT_LIGHT")
		{
			scene->add_light(std::make_shared<PointLight>(configuration));
		}

		if (configuration.type == "INFINITE_LIGHT")
		{
			scene->add_environment_light(std::make_shared<InfiniteLight>(configuration));
		}

		if (configuration.type == "AREA_LIGHT")
		{
			auto mesh = meshes.at(configuration.mesh_id);
			mesh->set_area_light(std::make_shared<AreaLight>(configuration, mesh));
			scene->add_light(mesh->get_area_light());
		}

		std::cout << "INFO::LOADER Light loaded -> [" << configuration.name << "]" << std::endl;
	}
}

void pbr::Loader::load_camera(const std::string& config, Camera* camera) const{

	std::ifstream ifs(config);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;

	document.ParseStream(isw);

	const rapidjson::Value& node = document["camera"];

	auto position = parser::Parser::string_to_vec3(node["position"].GetString());
	auto direction = parser::Parser::string_to_vec3(node["direction"].GetString());

	camera->get_coordinate().get().position = position;
	camera->get_coordinate().get().update(direction);
}

bool pbr::Loader::needs_reload(const std::string& config) const{

	return config != config_path;
}

void pbr::Loader::reload(const std::string& config, const std::shared_ptr<Scene>& scene){

	meshes.clear();
	textures_loaded.clear();
	config_path = config;

	glFinish();

	auto start = std::chrono::high_resolution_clock::now();

	load_meshes(config_path, scene.get());
	load_lights(config_path, scene.get());
	load_camera(config_path, scene->get_camera().get());

	scene->build();

	auto end = std::chrono::high_resolution_clock::now();
	const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "INFO::LOADER " << config_path << " loading time: [" << millis << " ms]" << std::endl;
}

void pbr::Loader::process_node(
	aiNode* node, const aiScene* ai_scene, parser::MeshConfig& configuration, Scene* scene){

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		auto mesh = process_mesh(ai_scene->mMeshes[node->mMeshes[i]], ai_scene, configuration);
		meshes[mesh->id] = mesh;
		scene->add_mesh(mesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		process_node(node->mChildren[i], ai_scene, configuration, scene);
}

std::shared_ptr<pbr::Mesh> pbr::Loader::process_mesh(aiMesh* mesh, const aiScene* scene,
                                                     parser::MeshConfig& configuration){

	std::vector<GL_Vertex> vertices(mesh->mNumVertices);
	std::vector<unsigned int> indices;
	std::vector<GL_Texture> textures;

	#pragma omp parallel num_threads(std::thread::hardware_concurrency())
	{
		#pragma omp for schedule(dynamic)
		for (long i = 0; i < mesh->mNumVertices; i++)
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

				vertex.tangent = vector;
			}

			if (mesh->mBitangents)
			{
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;

				vertex.bitangent = vector;
			}

			vertex.mesh_id = configuration.id;

			vertices[i] = vertex;
		}
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

	return std::make_shared<Mesh>(vertices, indices, textures, configuration);
}

std::vector<pbr::GL_Texture> pbr::Loader::load_material_textures(
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

unsigned int pbr::Loader::texture_from_file(const char* path, const std::string& directory, bool gamma){

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
