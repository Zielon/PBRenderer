#include "model.h"
#include <iostream>
#include <direct.h>

Model::Model(){
	loadModel();
}

void Model::loadModel() const{
	Assimp::Importer importer;

	char buff[FILENAME_MAX];
	_getcwd(buff, FILENAME_MAX);
	std::string dir(buff);

	std::string path = dir + "\\..\\resoruces\\nanosuit\\nanosuit.obj";

	const aiScene* scene =
		importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	std::string directory = path.substr(0, path.find_last_of('/'));
}
