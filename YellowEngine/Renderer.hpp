#ifndef __H_RENDERER__
#define __H_RENDERER__

#include "Component.hpp"

class Renderer : public Component
{
public:
	virtual void render() = 0;

protected:
	Renderer();
	virtual ~Renderer();

private:
};
#endif