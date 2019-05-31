#ifndef __H_COMPONENT__
#define __H_COMPONENT__

class GameObject;
class Transform;

class Component
{
	friend class GameObject;

public:
	GameObject* const gameObject;
	Transform* const transform;

	Component(GameObject* gameObject);
	virtual ~Component();

	virtual void onCreate();
	virtual void onDestroy();

	void setActive(bool active);
	bool getActive();

private:
	bool _active;
};

#endif