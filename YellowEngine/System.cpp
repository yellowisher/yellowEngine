#include "System.hpp"


System* System::_instance = nullptr;


System* System::getInstance()
{
	if (!_instance)
	{
		_instance = new System();
	}
	return _instance;
}


System::System()
{
}


System::System(const System& copy)
{
}


System::~System()
{
}


int System::getWidth()
{
	return _width;
}


int System::getHeight()
{
	return _height;
}


void System::setWidth(int width)
{
	_width = width;
}


void System::setHeight(int height)
{
	_height = height;
}


float System::getAspectRatio()
{
	return (float)_width / _height;
}