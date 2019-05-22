#ifndef __H_SHADERPROGRAME__
#define __H_SHADERPROGRAME__

#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"
#include "GameObject.hpp"
#include "AutoUniformBinding.hpp"

struct Uniform;

class ShaderProgram
{
public:
	static ShaderProgram* create(const char* vsPath, const char* fsPath);

	const Uniform* getUniform(std::string name);
	void setUniform(const Uniform* uniform, int value);
	void setUniform(const Uniform* uniform, const Vector2& value);
	void setUniform(const Uniform* uniform, const Vector3& value);
	void setUniform(const Uniform* uniform, const Vector4& value);
	void setUniform(const Uniform* uniform, const Matrix& value);

	void use(GameObject* user);

private:
	static std::map<std::string, ShaderProgram*> __shaderCache;

	unsigned int _id;
	std::map<std::string, Uniform> _uniforms;
	AutoUniformBinding _autoBinding;

	static ShaderProgram* createFromFile(const char* vsPath, const char* fsPath);
	static std::string readSourceFile(const char* path);

	ShaderProgram(int id);
	~ShaderProgram();
};

struct Uniform
{
	Uniform(std::string name, int index, GLenum type, int size, unsigned int handle);

	const std::string name;
	const int index;
	const GLenum type;
	const int size;
	const unsigned int handle;
};

#endif