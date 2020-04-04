#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "../rasterizer/shader.h"
#include "../rasterizer/mesh.h"

namespace general
{
	class Model
	{
	public:
		Model();

		void draw(const std::shared_ptr<rasterizer::Shader>& shader);

		void reload_model(int model);

	private:
		std::vector<rasterizer::Texture> textures_loaded;
		std::vector<rasterizer::Mesh> meshes;
		std::string directory;
		bool gamma_correction{};
		int current_model = -1;

		void load_model(const std::string &path);

		static unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

		void process_node(aiNode* node, const aiScene* scene);

		rasterizer::Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

		std::vector<rasterizer::Texture> load_material_textures(
			aiMaterial* mat, aiTextureType type, std::string type_name);
	};
}
