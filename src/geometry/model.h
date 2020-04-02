#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "gl_mesh.h"

namespace pbr
{
	class Model
	{
	public:
		Model();

		void draw(const Shader& shader);

	private:
		std::vector<Texture> textures_loaded;
		std::vector<GLMesh> meshes;
		std::string directory;
		bool gamma_correction;

		void loadModel();

		unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

		void process_node(aiNode* node, const aiScene* scene);

		GLMesh process_mesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);
	};
}
