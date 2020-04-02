#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <direct.h>

pbr::Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath){

	auto shaders = std::vector<std::tuple<std::string, GLuint, std::string>>{
		std::make_tuple(vertexPath, GL_VERTEX_SHADER, "VERTEX_SHADER"),
		std::make_tuple(fragmentPath, GL_FRAGMENT_SHADER, "FRAGMENT_SHADER"),
		std::make_tuple(geometryPath, GL_GEOMETRY_SHADER, "GEOMETRY_SHADER")
	};

	auto ids = std::vector<GLuint>();

	for (auto shader : shaders)
	{
		auto relativePath = std::get<0>(shader);

		if (relativePath.empty()) continue;

		auto path = "..\\src\\opengl\\glsl\\" + relativePath;
		auto type = std::get<1>(shader);
		auto name = std::get<2>(shader);

		std::ifstream shaderFile;
		std::string code;

		try
		{
			shaderFile.open(path);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			code = shaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cerr << "SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* shaderCode = code.c_str();

		int id = glCreateShader(type);
		glShaderSource(id, 1, &shaderCode, nullptr);
		glCompileShader(id);
		check_compile_errors(id, name);

		ids.push_back(id);
	}

	ID = glCreateProgram();

	for (auto shaderId : ids) glAttachShader(ID, shaderId);

	glLinkProgram(ID);
	check_compile_errors(ID, "PROGRAM");

	for (auto shaderId : ids) glDeleteShader(shaderId);
}

void pbr::Shader::check_compile_errors(GLuint shader, const std::string& type){

	GLint success;
	GLchar infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
				"\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
				"\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
