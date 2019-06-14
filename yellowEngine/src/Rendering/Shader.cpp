#include <iostream>
#include <fstream>
#include <sstream>

#include "yellowEngine/System/System.hpp"
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
			_uniforms.insert({ name, {name, type, size, location} });
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

		std::string vsPathString = System::getInstance()->getResourcePath(vsPath);
		std::string fsPathString = System::getInstance()->getResourcePath(fsPath);

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


	void Shader::setUniform(const Uniform* uniform, int value)
	{
		glUseProgram(_id);
		glUniform1i(uniform->location, value);
	}


	void Shader::setUniform(const Uniform* uniform, float value)
	{
		glUseProgram(_id);
		glUniform1f(uniform->location, value);
	}


	void Shader::setUniform(const Uniform* uniform, const Vector2& value)
	{
		glUseProgram(_id);
		glUniform2f(uniform->location, value.x, value.y);
	}


	void Shader::setUniform(const Uniform* uniform, const Vector3& value)
	{
		glUseProgram(_id);
		glUniform3f(uniform->location, value.x, value.y, value.z);
	}


	void Shader::setUniform(const Uniform* uniform, const Vector4& value)
	{
		glUseProgram(_id);
		glUniform4f(uniform->location, value.x, value.y, value.z, value.w);
	}


	void Shader::setUniform(const Uniform* uniform, const Matrix& value)
	{
		glUseProgram(_id);
		glUniformMatrix4fv(uniform->location, 1, GL_FALSE, value.getData());
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