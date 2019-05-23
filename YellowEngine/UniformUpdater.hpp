#ifndef __H_UNIFORMUPDATER__
#define __H_UNIFORMUPDATER__

#include <vector>

class ShaderProgram;
struct Uniform;

class UniformUpdater
{
	friend class StaticConstructor;

public:
	UniformUpdater(ShaderProgram* shader);
	~UniformUpdater();

	void initialize();
	void update(GameObject* user) const;

private:
	enum UniformType
	{
		Uniform_Model,
		Uniform_ProjectionView,
		Uniform_CameraPosition,
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
			__uniformStrings[Uniform_CameraPosition] = "u_CameraPosition";
		}
	};

	static StaticConstructor __staticConstructor;
	static const char* __uniformStrings[Num_Uniforms];

	ShaderProgram* _shader;
	std::vector<UniformPair> _uniformPairs;
};

#endif