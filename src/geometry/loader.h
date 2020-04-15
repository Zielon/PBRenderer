#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <utility>
#include <vector>

#include "../rasterizer/shader.h"
#include "../geometry/mesh.h"
#include "../core/scene.h"

namespace general
{
	class Loader
	{
	public:
		Loader(std::shared_ptr<pbr::Scene> scene): scene(std::move(scene)){

			load_meshes("../configuration/default.json");
			load_lights("../configuration/default.json");
		};

		void load_meshes(const std::string& config);

		void load_lights(const std::string& config) const;

	private:
		std::string directory;
		std::vector<GL_Texture> textures_loaded;
		std::shared_ptr<pbr::Scene> scene;

		static unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

		void process_node(aiNode* node, const aiScene* scene, parser::MeshConfig& configuration);

		std::shared_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene, parser::MeshConfig& configuration);

		std::vector<GL_Texture> load_material_textures(
			aiMaterial* mat, aiTextureType type, std::string type_name);
	};
}
