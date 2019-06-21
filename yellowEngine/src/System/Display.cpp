#include "yellowEngine/System/Display.hpp"


namespace yellowEngine
{
	const int& Display::width         = _width;
	const int& Display::height        = _height;
	const float& Display::aspectRatio = _aspectRatio;

	int Display::_width;
	int Display::_height;
	float Display::_aspectRatio;
}