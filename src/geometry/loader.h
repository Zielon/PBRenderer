#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <utility>
#include <vector>

#include "../rasterizer/shader.h"
#include "../geometry/mesh.h"
#include "../core/scene.h"
#include <map>

namespace pbr
{
	class Loader
	{
	public:
		Loader(std::shared_ptr<Scene> scene, std::string path):
			config_path(std::move(path)),
			scene(std::move(scene)){

			auto start = std::chrono::high_resolution_clock::now();

			load_meshes(config_path);
			load_lights(config_path);

			auto end = std::chrono::high_resolution_clock::now();
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "INFO::LOADER " << config_path << " loading time: [" << millis << " ms]" << std::endl;
		};

		~Loader(){

			scene = nullptr;
			meshes.clear();
		}

		void load_meshes(const std::string& config);

		void load_lights(const std::string& config);

		std::string config_path;

	private:
		std::string directory;
		std::vector<GL_Texture> textures_loaded;
		std::shared_ptr<Scene> scene;
		std::map<int, std::shared_ptr<Mesh>> meshes;

		static unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

		void process_node(aiNode* node, const aiScene* scene, parser::MeshConfig& configuration);

		std::shared_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene, parser::MeshConfig& configuration);

		std::vector<GL_Texture> load_material_textures(
			aiMaterial* mat, aiTextureType type, std::string type_name);
	};
}
