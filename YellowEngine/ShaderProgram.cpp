using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

#include "ShaderProgram.hpp"

map<string, ShaderProgram*> ShaderProgram::__shaderCache;

ShaderProgram::ShaderProgram()
{
}


ShaderProgram::ShaderProgram(int id) :_id(id)
{
}


ShaderProgram::~ShaderProgram()
{
}


ShaderProgram* ShaderProgram::create(const char* vsPath, const char* psPath)
{
	string path = vsPath;
	path += psPath;

	auto it = __shaderCache.find(path);
	if (it != __shaderCache.end())
	{
		return it->second;
	}

	ShaderProgram* shaderProgram = createFromFile(vsPath, psPath);
	if (shaderProgram != nullptr)__shaderCache.insert({ path, shaderProgram });
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


unsigned int ShaderProgram::getUniformHandle(string uniform)
{
	auto it = _uniformHandles.find(uniform);
	if (it != _uniformHandles.end())
	{
		return it->second;
	}

	unsigned int handle = glGetUniformLocation(_id, uniform.c_str());
	if (handle != -1)
	{
		_uniformHandles.insert({ uniform,handle });
	}
	return handle;
}


void ShaderProgram::setUniform(unsigned int handle, int value)
{
	glUseProgram(_id);
	glUniform1i(handle, value);
}


void ShaderProgram::setUniform(unsigned int handle, const Vector2& value)
{
	glUseProgram(_id);
	glUniform2f(handle, value.x, value.y);
}


void ShaderProgram::setUniform(unsigned int handle, const Vector3& value)
{
	glUseProgram(_id);
	glUniform3f(handle, value.x, value.y, value.z);
}


void ShaderProgram::setUniform(unsigned int handle, const Vector4& value)
{
	glUseProgram(_id);
	glUniform4f(handle, value.x, value.y, value.z, value.w);
}


void ShaderProgram::setUniform(unsigned int handle, const Matrix& value)
{
	glUseProgram(_id);
	glUniformMatrix4fv(handle, 1, GL_FALSE, value.m);
}


void ShaderProgram::use()
{
	glUseProgram(_id);
}
