#include "yellowEngine/System/System.hpp"


namespace yellowEngine
{
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


	System::~System()
	{
	}


	float System::getWidth()
	{
		return _width;
	}


	float System::getHeight()
	{
		return _height;
	}


	void System::setWidth(float width)
	{
		_width = width;
	}


	void System::setHeight(float height)
	{
		_height = height;
	}


	float System::getAspectRatio()
	{
		return _width / _height;
	}


	void System::setResourcePath(const char* path)
	{
		_resourcePath = path;
	}


	const std::string System::getResourcePath(const char* fileName)
	{
		std::string path = _resourcePath;
		path += fileName;
		return path;
	}
}