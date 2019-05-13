using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

#include "ShaderProgram.hpp"

map<string, ShaderProgram*> ShaderProgram::shaderPrograms;

ShaderProgram::ShaderProgram()
{
}


ShaderProgram::ShaderProgram(int id) :id(id)
{
}


ShaderProgram::~ShaderProgram()
{
}


ShaderProgram* ShaderProgram::create(const char* vsPath, const char* psPath)
{
	string path = vsPath;
	path += psPath;

	auto it = shaderPrograms.find(path);
	if (it != shaderPrograms.end())
	{
		return it->second;
	}

	ShaderProgram* shaderProgram = createFromFile(vsPath, psPath);
	if (shaderProgram != nullptr)shaderPrograms.insert({ path, shaderProgram });
	return shaderProgram;
}


ShaderProgram* ShaderProgram::createFromFile(const char* vsPath, const char* psPath)
{
	unsigned int vs, ps;
	int success;
	char log[512];
	string shader;
	const char* shaderCode;

	shader = readSourceFile(vsPath);
	shaderCode = shader.c_str();
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &shaderCode, NULL);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, log);
		cout << "Vertex shader compile error\n" << log << endl;
		return nullptr;
	}

	shader = readSourceFile(psPath);
	shaderCode = shader.c_str();
	ps = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ps, 1, &shaderCode, NULL);
	glCompileShader(ps);

	glGetShaderiv(ps, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ps, 512, NULL, log);
		cout << "Pixel shader compile error\n" << log << endl;
		return nullptr;
	}

	// linking shader program
	int id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, ps);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, log);
		cout << "Shader programe link error\n" << log << endl;
	}

	glDeleteShader(vs);
	glDeleteShader(ps);
	return new ShaderProgram(id);
}


string ShaderProgram::readSourceFile(const char* path)
{
	ifstream ifs;
	ifs.open(path);
	if (ifs.fail())
	{
		cout << "Cannot open file " << path << endl;
		return nullptr;
	}

	stringstream stream;
	stream << ifs.rdbuf();
	ifs.close();
	return stream.str();
}


void ShaderProgram::use()
{
	glUseProgram(id);
}
