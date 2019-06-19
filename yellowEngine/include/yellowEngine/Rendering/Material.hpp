#ifndef __H_MATERIAL__
#define __H_MATERIAL__

#include <vector>
#include <cstring>

#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/Texture.hpp"
#include "yellowEngine/Rendering/Technique.hpp"

namespace yellowEngine
{
	class Material
	{
		friend class MeshRenderer;
		friend class Technique;

	public:
		struct Property
		{
			Property() {};
			Property(const Property& copy) { memcpy(this, &copy, sizeof(Property)); }
			~Property() {};

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

		Material();
		~Material();
		void setTechnique(Technique* technique, const char* defaultVsPath, const char* defaultFsPath);
		Material* attachTo(GameObject* gameObject, Mesh* mesh);
		void addTexture(Texture* texture, const char* usage);
		void bind(const char* vsPath = nullptr, const char* fsPath = nullptr);
		void unbind();

		void setProperty(const char* name, int value);
		void setProperty(const char* name, float value);
		void setProperty(const char* name, Vector2 value);
		void setProperty(const char* name, Vector3 value);
		void setProperty(const char* name, Vector4 value);
		void setProperty(const char* name, Matrix value);

	private:
		GameObject* _gameObject;
		Mesh* _mesh;

		Technique* _technique;
		const char* _defaultVsPath;
		const char* _defaultFsPath;

		std::map<std::string, Texture*> _textures;
		std::map<std::string, Property> _properties;
	};
}

#endif