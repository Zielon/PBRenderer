#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <utility>
#include <vector>

#include "../geometry/mesh.h"
#include "../core/scene.h"
#include <map>

namespace pbr
{
	class Loader
	{
	public:
		Loader(){}

		Loader(const std::shared_ptr<Scene>& scene, std::string path):
			config_path(std::move(path)){

			reload(config_path, scene);
		};

		bool needs_reload(const std::string& config) const;

		void reload(const std::string& config, const std::shared_ptr<Scene>& scene);

	private:
		std::string config_path;
		std::string directory;
		std::vector<GL_Texture> textures_loaded;
		std::map<int, std::shared_ptr<Mesh>> meshes;

		void load_meshes(const std::string& config, Scene* scene);

		void load_lights(const std::string& config, Scene* scene);

		static unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

		void process_node(aiNode* node, const aiScene* scene, parser::MeshConfig& configuration, Scene* mesh_scene);

		std::shared_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene, parser::MeshConfig& configuration);

		std::vector<GL_Texture> load_material_textures(
			aiMaterial* mat, aiTextureType type, std::string type_name);
	};
}
