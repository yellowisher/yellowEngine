#ifndef __H_SHADER__
#define __H_SHADER__

#include <string>
#include <vector>
#include <map>
#include <glad/glad.h>

#include "yellowEngine/Math/Vector2.hpp"
#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Math/Vector4.hpp"
#include "yellowEngine/Math/Matrix.hpp"
#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Rendering/UniformUpdater.hpp"

namespace yellowEngine
{
	struct Attribute
	{
		std::string name;
		GLenum type;
		int size;
		int location;
	};

	struct Uniform
	{
		std::string name;
		GLenum type;
		int size;
		int location;
	};

	class Shader
	{
	public:
		static Shader* create(const char* vsPath, const char* fsPath);

		// TODO: remove this; for forward lighting only
		int getId() { return _id; }

		const std::vector<Attribute>& getAttributes();
		const Uniform* getUniform(std::string name);
		void setUniform(const Uniform* uniform, int value);
		void setUniform(const Uniform* uniform, float value);
		void setUniform(const Uniform* uniform, const Vector2& value);
		void setUniform(const Uniform* uniform, const Vector3& value);
		void setUniform(const Uniform* uniform, const Vector4& value);
		void setUniform(const Uniform* uniform, const Matrix& value);

		void bind();
		void unbind();
		void updateUniforms(GameObject* target);

	private:
		static std::map<std::string, Shader*> __shaderCache;

		unsigned int _id;
		std::vector<Attribute> _attributes;
		std::map<std::string, Uniform> _uniforms;
		UniformUpdater _uniformUpdater;

		static Shader* createFromFile(const char* vsPath, const char* fsPath);
		static std::string readSourceFile(const char* path);

		Shader(int id);
		~Shader();
	};
}

#endif