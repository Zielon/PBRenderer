#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "json/json.h"

#include "../rasterizer/shader.h"
#include "../geometry/mesh.h"

namespace general
{
	class Model_loader
	{
	public:
		Model_loader();

		void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe);

		void reload_model(int model);

	//private:
		std::vector<GL_Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory;
		int current_model = -1;

		void load_model(const std::string& path);

		static unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

		void process_node(aiNode* node, const aiScene* scene);

		Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

		std::vector<GL_Texture> load_material_textures(
			aiMaterial* mat, aiTextureType type, std::string type_name);
	};
}
