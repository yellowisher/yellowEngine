#ifndef __H_SHADERPROGRAME__
#define __H_SHADERPROGRAME__

#include <string>
#include <map>

class ShaderProgram
{
public:
	static ShaderProgram* create(const char* vsPath, const char* psPath);

	void use();

private:
	static map<string, ShaderProgram*> shaderPrograms;
	unsigned int id;

	static ShaderProgram* createFromFile(const char* vsPath, const char* psPath);
	static string readSourceFile(const char* path);

	ShaderProgram();
	ShaderProgram(int id);
	~ShaderProgram();
};

#endif