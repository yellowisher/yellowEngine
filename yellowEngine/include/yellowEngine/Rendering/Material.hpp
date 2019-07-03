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
	class MeshRenderer;

	class Material
	{
		friend class Model;
		friend class MeshRenderer;
		friend class Technique;

	public:
		enum Primitive
		{
			Primitive_Float,
			Primitive_Texture,
			Primitive_Color
		};

		static std::map<std::string, Primitive> stringToPrimitive;

		struct Property
		{
			Property() {};
			Property(const Property& copy) { type = copy.type; colorValue = copy.colorValue; }
			~Property() {};

			std::string type;
			union
			{
				int intValue;
				float floatValue;
				Vector3 colorValue;
			};
		};

		static Material* create(const char* path);
		static void saveAsFile(Material* material, const char* path);

		void setTechnique(Technique* technique, const char* defaultVsPath, const char* defaultFsPath);
		void bind(MeshRenderer* meshRenderer, const char* vsPath, const char* fsPath);
		void unbind();
		std::string getPath();
		std::map<std::string, Property> getProperties() { return _properties; }
		std::map<std::string, Texture*> getTextures() { return _textures; };

		void setProperty(const char* name, Texture* texture);
		void setProperty(const char* name, int value);
		void setProperty(const char* name, float value);
		void setProperty(const char* name, Vector3 value);
		
	private:
		static std::map<std::string, Material*> _materialCache;

		Material(const char* path);
		~Material();

		std::string _path;
		Technique* _technique;
		const char* _defaultVsPath;
		const char* _defaultFsPath;

		std::map<std::string, Texture*> _textures;
		std::map<std::string, Property> _properties;
	};
}

#endif