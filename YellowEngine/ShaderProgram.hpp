#ifndef __H_SHADERPROGRAME__
#define __H_SHADERPROGRAME__

#include <string>
#include <map>

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"

class ShaderProgram
{
public:
	static ShaderProgram* create(const char* vsPath, const char* psPath);

	unsigned int getUniformHandle(std::string uniform);
	void setUniform(unsigned int handle, int value);
	void setUniform(unsigned int handle, Vector2& value);
	void setUniform(unsigned int handle, Vector3& value);
	void setUniform(unsigned int handle, Vector4& value);
	void setUniform(unsigned int handle, Matrix& value);

	void use();

private:
	static std::map<std::string, ShaderProgram*> __shaderCache;

	std::map<std::string, int> _uniformHandles;
	unsigned int _id;

	static ShaderProgram* createFromFile(const char* vsPath, const char* psPath);
	static std::string readSourceFile(const char* path);

	ShaderProgram();
	ShaderProgram(int id);
	~ShaderProgram();
};

#endif