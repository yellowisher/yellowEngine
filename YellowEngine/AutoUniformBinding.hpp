#ifndef __H_AUTOUNIFORMBINDING__
#define __H_AUTOUNIFORMBINDING__

#include <vector>

class ShaderProgram;
struct Uniform;

class AutoUniformBinding
{
	friend class StaticConstructor;

public:
	AutoUniformBinding(ShaderProgram* shader);
	~AutoUniformBinding();

	void bind(GameObject* user) const;

private:
	enum UniformType
	{
		Uniform_Model,
		Uniform_ProjectionView,
		Uniform_ModelColor,
		Uniform_LightColor,
		Num_Uniforms
	};

	// instead just use std::pair?
	struct UniformPair
	{
		UniformType type;
		const Uniform* uniform;
	};

	struct StaticConstructor
	{
		StaticConstructor()
		{
			__uniformStrings[Uniform_Model] = "u_Model";
			__uniformStrings[Uniform_ProjectionView] = "u_ProjectionView";
			__uniformStrings[Uniform_ModelColor] = "u_ModelColor";
			__uniformStrings[Uniform_LightColor] = "u_LightColor";
		}
	};

	static StaticConstructor __staticConstructor;
	static const char* __uniformStrings[Num_Uniforms];

	ShaderProgram* _shader;
	std::vector<UniformPair> _uniformPairs;
};

#endif