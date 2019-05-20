#ifndef __H_RENDERER__
#define __H_RENDERER__

#include "Component.hpp"

class Renderer : public Component
{
public:
	Renderer(GameObject* gameObject);
	virtual ~Renderer();

	virtual void render() = 0;

};
#endif