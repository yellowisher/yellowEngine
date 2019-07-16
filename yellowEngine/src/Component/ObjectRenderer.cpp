#include "yellowEngine/Component/ObjectRenderer.hpp"
#include "yellowEngine/Component/GameObject.hpp"


namespace yellowEngine
{
	std::list<ObjectRenderer*> ObjectRenderer::_renderers;


	ObjectRenderer::ObjectRenderer(GameObject* gameObject) :Component(gameObject)
	{
		_renderers.push_back(this);
	}


	ObjectRenderer::~ObjectRenderer()
	{
		for (auto it = _renderers.begin(); it != _renderers.end(); ++it)
		{
			if (*it == this)
			{
				_renderers.erase(it);
				return;
			}
		}
	}


	void ObjectRenderer::render(const char* vsPath, const char* fsPath)
	{
		_render(vsPath, fsPath);
	}


	void ObjectRenderer::renderAll(const char* vsPath, const char* fsPath)
	{
		for (auto renderer : _renderers)
		{
			if (renderer->getActive())
			{
				renderer->render(vsPath, fsPath);
			}
		}
	}
}