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
		Loader(std::shared_ptr<Scene> scene): scene(std::move(scene)){

			auto start = std::chrono::high_resolution_clock::now();

			load_meshes("../configuration/default.json");
			load_lights("../configuration/default.json");

			auto end = std::chrono::high_resolution_clock::now();
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "INFO::LOADER Config loading time: [" << millis << " ms]" << std::endl;
		};

		void load_meshes(const std::string& config);

		void load_lights(const std::string& config) const;

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
