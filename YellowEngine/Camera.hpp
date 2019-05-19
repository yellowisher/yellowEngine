#ifndef __H_CAMERA__
#define __H_CAMERA__

#include "Component.hpp"
#include "Matrix.hpp"

class Camera : public Component
{
public:
	enum Type
	{
		Perspective,
		Orthographic
	};

private:
	Type _type;

};

#endif