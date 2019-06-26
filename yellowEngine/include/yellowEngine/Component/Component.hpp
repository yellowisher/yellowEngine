#ifndef __H_COMPONENT__
#define __H_COMPONENT__

#include <map>
#include <string>

namespace yellowEngine
{
	class GameObject;
	class Transform;

	class Component
	{
		friend class GameObject;

	public:
		static Component* createComponent(const std::string& type, GameObject* gameObject);

		GameObject* const gameObject;
		Transform* const transform;

		Component(GameObject* gameObject);
		virtual ~Component();

		virtual void onCreate();
		virtual void onDestroy();

		void setActive(bool active);
		bool getActive();

	protected:
		// store createComponent function pointers that can be referenced with string
		template<class T> static Component* createComponent(GameObject* gameObject) { return new T(gameObject); }
		static std::map <std::string, Component*(*)(GameObject*)> __constructors;

	private:
		bool _active;
	};
}

#endif