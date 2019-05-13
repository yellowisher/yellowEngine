#ifndef __H_RENDERER__
#define __H_RENDERER__

#include "Component.hpp"

class Renderer : Component
{
public:
	Renderer();
	virtual ~Renderer();

	virtual void render() = 0;

private:
};
#endif