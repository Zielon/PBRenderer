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
	class ModelLoader
	{
	public:
		ModelLoader(std::shared_ptr<pbr::Scene> scene): scene(std::move(scene)){

			load_models("../configuration/default.json");
		};

		void draw(const std::shared_ptr<rasterizer::Shader>& shader, bool wireframe);

		void load_models(const std::string& config);

		void reload_model(int model);

	private:
		std::string directory;
		std::vector<GL_Texture> textures_loaded;
		int current_config = -1;

		std::shared_ptr<pbr::Scene> scene;

		static unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

		void process_node(aiNode* node, const aiScene* scene, pbr::Transformation& transformation, int mesh_id);

		std::shared_ptr<Mesh> process_mesh(aiMesh* mesh, const aiScene* scene, pbr::Transformation& transformation, int mesh_id);

		std::vector<GL_Texture> load_material_textures(
			aiMaterial* mat, aiTextureType type, std::string type_name);

		std::vector<std::string> split_string(const std::string& s, char delimiter) const;

		glm::vec3 string_to_vec3(const std::string& s) const;
	};
}
