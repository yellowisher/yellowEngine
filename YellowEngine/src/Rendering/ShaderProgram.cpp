#include <iostream>
#include <fstream>
#include <sstream>

#include "yellowEngine/System/System.hpp"
#include "yellowEngine/Rendering/ShaderProgram.hpp"

std::map<std::string, ShaderProgram*> ShaderProgram::__shaderCache;

ShaderProgram::ShaderProgram(int id) :_id(id), _uniformUpdater(this)
{
	int count, size, length, handle;
	const int bufferSize = 32;
	char name[bufferSize];
	GLenum type;

	glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(_id, i, bufferSize, &length, &size, &type, name);
		handle = glGetUniformLocation(_id, name);
		_uniforms.insert({ name, Uniform(name, i, type, size, handle) });
	}

	_uniformUpdater.initialize();
}


ShaderProgram::~ShaderProgram()
{
}


// maybe unordered_map would be better
ShaderProgram* ShaderProgram::create(const char* vsPath, const char* fsPath)
{
	std::string path = vsPath;
	path += fsPath;

	auto it = __shaderCache.find(path);
	if (it != __shaderCache.end())
	{
		return it->second;
	}

	ShaderProgram* shaderProgram = createFromFile(vsPath, fsPath);
	if (shaderProgram != nullptr)__shaderCache.insert({ path, shaderProgram });
	return shaderProgram;
}


ShaderProgram* ShaderProgram::createFromFile(const char* vsPath, const char* fsPath)
{
	unsigned int vs, fs;
	int success;
	char log[512];
	std::string shader;
	const char* shaderCode;

	std::string vsPathString = System::getInstance()->getResourcePath(vsPath).c_str();
	std::string fsPathString = System::getInstance()->getResourcePath(fsPath).c_str();

	shader = readSourceFile(vsPathString.c_str());
	shaderCode = shader.c_str();
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &shaderCode, NULL);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, log);
		std::cout << "Vertex shader compile error\n" << log << std::endl;
		return nullptr;
	}

	shader = readSourceFile(fsPathString.c_str());
	shaderCode = shader.c_str();
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &shaderCode, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, log);
		std::cout << "Pixel shader compile error\n" << log << std::endl;
		return nullptr;
	}

	// linking shader program
	int id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, log);
		std::cout << "Shader programe link error\n" << log << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return new ShaderProgram(id);
}


std::string ShaderProgram::readSourceFile(const char* path)
{
	std::ifstream ifs;
	ifs.open(path);
	if (ifs.fail())
	{
		std::cout << "Cannot open file " << path << std::endl;
		return nullptr;
	}

	std::stringstream stream;
	stream << ifs.rdbuf();
	ifs.close();
	return stream.str();
}


const Uniform* ShaderProgram::getUniform(std::string name)
{
	auto it = _uniforms.find(name);
	if (it != _uniforms.end())
	{
		return &it->second;
	}
	return nullptr;
}


void ShaderProgram::setUniform(const Uniform* uniform, int value)
{
	glUseProgram(_id);
	glUniform1i(uniform->handle, value);
}


void ShaderProgram::setUniform(const Uniform* uniform, float value)
{
	glUseProgram(_id);
	glUniform1f(uniform->handle, value);
}


void ShaderProgram::setUniform(const Uniform* uniform, const Vector2& value)
{
	glUseProgram(_id);
	glUniform2f(uniform->handle, value.x, value.y);
}


void ShaderProgram::setUniform(const Uniform* uniform, const Vector3& value)
{
	glUseProgram(_id);
	glUniform3f(uniform->handle, value.x, value.y, value.z);
}


void ShaderProgram::setUniform(const Uniform* uniform, const Vector4& value)
{
	glUseProgram(_id);
	glUniform4f(uniform->handle, value.x, value.y, value.z, value.w);
}


void ShaderProgram::setUniform(const Uniform* uniform, const Matrix& value)
{
	glUseProgram(_id);
	glUniformMatrix4fv(uniform->handle, 1, GL_FALSE, value.m);
}


void ShaderProgram::use()
{
	glUseProgram(_id);
}


void ShaderProgram::updateUniforms(GameObject* target)
{
	glUseProgram(_id);
	_uniformUpdater.update(target);
}


int ShaderProgram::getId()
{
	return _id;
}


Uniform::Uniform(std::string name, int index, GLenum type, int size, unsigned int handle) :
	name(name),
	index(index),
	type(type),
	size(size),
	handle(handle)
{
}