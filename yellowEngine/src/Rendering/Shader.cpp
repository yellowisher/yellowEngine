#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#include "yellowEngine/System/Game.hpp"
#include "yellowEngine/Rendering/Shader.hpp"


namespace yellowEngine
{
	std::map<std::string, Shader*> Shader::__shaderCache;

	Shader::Shader(int id) :_id(id), _uniformUpdater(this)
	{
		int count, size, length, location;
		const int bufferSize = 32;
		char name[bufferSize];
		GLenum type;

		glUseProgram(_id);
		glGetProgramiv(_id, GL_ACTIVE_ATTRIBUTES, &count);
		for (int i = 0; i < count; i++)
		{
			glGetActiveAttrib(_id, i, bufferSize, &length, &size, &type, name);
			location = glGetAttribLocation(_id, name);
			_attributes.push_back({ name, type, size, location });
		}

		glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &count);
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(_id, i, bufferSize, &length, &size, &type, name);
			location = glGetUniformLocation(_id, name);

			if (type == GL_SAMPLER_1D || type == GL_SAMPLER_2D || type == GL_SAMPLER_3D || type == GL_SAMPLER_CUBE)
			{
				glUniform1i(location, (int)_textureUnits.size());
				_textureUnits.push_back(name);
			}
			else
			{
				_uniforms.insert({ name, {name, type, size, location} });
			}
		}

		_uniformUpdater.init();
	}


	Shader::~Shader()
	{
	}


	// maybe unordered_map would be better
	Shader* Shader::create(const char* vsPath, const char* fsPath)
	{
		std::string path = vsPath;
		path += fsPath;

		auto it = __shaderCache.find(path);
		if (it != __shaderCache.end())
		{
			return it->second;
		}

		Shader* Shader = createFromFile(vsPath, fsPath);
		if (Shader != nullptr)__shaderCache.insert({ path, Shader });
		return Shader;
	}


	Shader* Shader::createFromFile(const char* vsPath, const char* fsPath)
	{
		unsigned int vs, fs;
		int success;
		char log[512];
		std::string shader;
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
			std::cout << "Vertex shader compile error\n" << log << std::endl;
			return nullptr;
		}

		shader = readSourceFile(fsPath);
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
		return new Shader(id);
	}


	std::string Shader::readSourceFile(const char* path)
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


	const std::vector<Attribute>& Shader::getAttributes()
	{
		return _attributes;
	}


	const std::vector<std::string>& Shader::getTextureUnits()
	{
		return _textureUnits;
	}


	const std::map<std::string, Uniform>& Shader::getUniforms()
	{
		return _uniforms;
	}


	const Uniform* Shader::getUniform(std::string name)
	{
		auto it = _uniforms.find(name);
		if (it != _uniforms.end())
		{
			return &it->second;
		}
		return nullptr;
	}

	void Shader::setUniform(const std::string& name, int value, int index)
	{
		assert(_uniforms.find(name) != _uniforms.end());
		setUniform(&_uniforms[name], value, index);
	}


	void Shader::setUniform(const std::string& name, float value, int index)
	{
		assert(_uniforms.find(name) != _uniforms.end());
		setUniform(&_uniforms[name], value, index);
	}


	void Shader::setUniform(const std::string& name, const Vector2 & value, int index)
	{
		assert(_uniforms.find(name) != _uniforms.end());
		setUniform(&_uniforms[name], value, index);
	}


	void Shader::setUniform(const std::string& name, const Vector3 & value, int index)
	{
		assert(_uniforms.find(name) != _uniforms.end());
		setUniform(&_uniforms[name], value, index);
	}


	void Shader::setUniform(const std::string& name, const Vector4 & value, int index)
	{
		assert(_uniforms.find(name) != _uniforms.end());
		setUniform(&_uniforms[name], value, index);
	}


	void Shader::setUniform(const std::string& name, const Matrix & value, int index)
	{
		assert(_uniforms.find(name) != _uniforms.end());
		setUniform(&_uniforms[name], value, index);
	}


	void Shader::setUniform(const Uniform* uniform, int value, int index)
	{
		glUseProgram(_id);
		glUniform1i(uniform->location + index, value);
	}


	void Shader::setUniform(const Uniform* uniform, float value, int index)
	{
		glUseProgram(_id);
		glUniform1f(uniform->location + index, value);
	}


	void Shader::setUniform(const Uniform* uniform, const Vector2& value, int index)
	{
		glUseProgram(_id);
		glUniform2f(uniform->location + index, value.x, value.y);
	}


	void Shader::setUniform(const Uniform* uniform, const Vector3& value, int index)
	{
		glUseProgram(_id);
		glUniform3f(uniform->location + index, value.x, value.y, value.z);
	}


	void Shader::setUniform(const Uniform* uniform, const Vector4& value, int index)
	{
		glUseProgram(_id);
		glUniform4f(uniform->location + index, value.x, value.y, value.z, value.w);
	}


	void Shader::setUniform(const Uniform* uniform, const Matrix& value, int index)
	{
		glUseProgram(_id);
		glUniformMatrix4fv(uniform->location + index, 1, GL_FALSE, value.getData());
	}


	void Shader::bind()
	{
		glUseProgram(_id);
	}


	void Shader::unbind()
	{
		glUseProgram(NULL);
	}


	void Shader::updateUniforms(GameObject* target)
	{
		_uniformUpdater.update(target);
	}
}