#ifndef __H_MATERIAL__
#define __H_MATERIAL__

#include <vector>
#include <cstring>

#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Rendering/Texture.hpp"
#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"

namespace yellowEngine
{
	class Material
	{
	public:
		struct Property
		{
			Property() {};
			Property(const Property& copy) { memcpy(this, &copy, sizeof(Property)); }
			~Property() {};

			const Uniform* uniform;
			union
			{
				int intValue;
				float floatValue;
				Vector2 vector2Value;
				Vector3 vector3Value;
				Vector4 vector4Value;
				Matrix matrixValue;
			};
		};

		Material() {};
		Material(Shader* shader);
		~Material();
		Material* init(GameObject* gameObject, Mesh* mesh);
		void addTexture(Texture* texture, const char* usage);
		void bind();
		void unbind();
		Shader* getShader();

		void setProperty(const char* name, int value);
		void setProperty(const char* name, float value);
		void setProperty(const char* name, Vector2 value);
		void setProperty(const char* name, Vector3 value);
		void setProperty(const char* name, Vector4 value);
		void setProperty(const char* name, Matrix value);

	private:

		GameObject* _gameObject;
		Shader* _shader;
		VertexLayoutBinding* _binding;

		std::vector<std::pair<std::string, Texture*>> _textures;
		std::map<std::string, Property> _properties;
	};
}

#endif