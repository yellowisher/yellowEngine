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
		int getTextureUnit(const std::string& name);
		const std::vector<std::string>& getTextureUnits();
		const std::map<std::string, Uniform>& getUniforms();
		const Uniform* getUniform(std::string name);

		void setUniform(const std::string& name, int value, int index = 0);
		void setUniform(const std::string& name, float value, int index = 0);
		void setUniform(const std::string& name, const Vector2& value, int index = 0);
		void setUniform(const std::string& name, const Vector3& value, int index = 0);
		void setUniform(const std::string& name, const Vector4& value, int index = 0);
		void setUniform(const std::string& name, const Matrix& value, int index = 0);

		void setUniform(const Uniform* uniform, int value, int index = 0);
		void setUniform(const Uniform* uniform, float value, int index = 0);
		void setUniform(const Uniform* uniform, const Vector2& value, int index = 0);
		void setUniform(const Uniform* uniform, const Vector3& value, int index = 0);
		void setUniform(const Uniform* uniform, const Vector4& value, int index = 0);
		void setUniform(const Uniform* uniform, const Matrix& value, int index = 0);

		void bind();
		void unbind();
		void updateUniforms(GameObject* target);

	private:
		static std::map<std::string, Shader*> __shaderCache;

		unsigned int _id;
		std::vector<Attribute> _attributes;
		std::vector<std::string> _textureUnits;
		std::map<std::string, Uniform> _uniforms;
		UniformUpdater _uniformUpdater;

		static Shader* createFromFile(const char* vsPath, const char* fsPath);
		static std::string readSourceFile(const char* path);

		Shader(int id);
		~Shader();
	};
}

#endif