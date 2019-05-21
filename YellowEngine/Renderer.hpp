#ifndef __H_RENDERER__
#define __H_RENDERER__

#include <list>

#include "Component.hpp"
#include "Camera.hpp"

class Renderer : public Component
{
public:
	static void renderAll(Camera* camera);

	Renderer(GameObject* gameObject);
	virtual ~Renderer();

	void render();

protected:
	static Camera* _currentCamera;

	virtual void _render() = 0;

private:
	static std::list<Renderer*> _renderers;

};
#endif