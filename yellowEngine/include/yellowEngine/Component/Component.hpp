#ifndef __H_COMPONENT__
#define __H_COMPONENT__

#include <map>
#include <vector>
#include <string>
#include <cstddef>

#define BEGIN_COMPONENT(cls) \
friend class Component;\
	private:\
		static const char* __typeName;\
	public:\
		virtual const char* getTypeName() override { return __typeName; }\
	private:\
		struct _StaticConstructor\
		{\
			_StaticConstructor()\
			{\
				Component::getConstructors()[#cls] = Component::createComponent<cls>;\
				Component::getComponents().push_back(#cls);\
				Component::getProperties().insert({ #cls, {} });\
				auto& offsets = Component::getProperties()[#cls];

#define PROPERTY(cls, type, field, name) \
				offsets.push_back({ #type, name, offsetof(cls, field) });

#define END_COMPONENT \
			}\
		};\
		static const _StaticConstructor __sc;

#define COMPONENT_IMPL(cls) \
	const cls::_StaticConstructor cls::__sc;\
	const char* cls::__typeName = #cls;


int main();

namespace yellowEngine
{
	class GameObject;
	class Transform;

	class Component
	{
		friend class GameObject;
		friend int ::main();

	public:
		struct Property
		{
			std::string type;
			std::string name;
			size_t offset;
		};


		// TODO: move RTTI to each component, not hold as static method in Component
		static Component* createComponent(const std::string& type, GameObject* gameObject);
		static std::map<std::string, Component*(*)(GameObject*)>& getConstructors();
		static std::vector<std::string>& getComponents();
		static std::map<std::string, std::vector<Property>>& getProperties();
		template <class T>
		static const char* getTypeNameOf() { return T::__typeName; }

		GameObject* const gameObject;
		Transform* const transform;

		Component(GameObject* gameObject);
		virtual ~Component();

		virtual void onCreate() {};
		virtual void onDestroy() {};
		virtual void onValueChanged() {};

		virtual const char* getTypeName() { return nullptr; }
		void setActive(bool active);
		bool getActive();

	protected:
		// store createComponent function pointers that can be referenced with string
		template<class T> static Component* createComponent(GameObject* gameObject) { return new T(gameObject); }

	private:
		bool _active;
	};
}

#endif