#ifndef __H_OBJECTRENDERER__
#define __H_OBJECTRENDERER__

#include <list>

#include "Component.hpp"
#include "Camera.hpp"

class ObjectRenderer : public Component
{
public:
	static void renderAll(Camera* camera);
	static Camera* getCurrentCamera();

	ObjectRenderer(GameObject* gameObject);
	virtual ~ObjectRenderer();

	void render();

	static Camera* _currentCamera;
protected:

	virtual void _render() = 0;

private:
	static std::list<ObjectRenderer*> _renderers;

};
#endif