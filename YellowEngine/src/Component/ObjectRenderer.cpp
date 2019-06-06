#include "yellowEngine/Component/ObjectRenderer.hpp"
#include "yellowEngine/System/GameObject.hpp"


namespace yellowEngine
{
	std::list<ObjectRenderer*> ObjectRenderer::_renderers;
	Camera* ObjectRenderer::_currentCamera;


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


	void ObjectRenderer::render()
	{
		_render();
	}


	void ObjectRenderer::renderAll(Camera* camera)
	{
		_currentCamera = camera;
		for (auto renderer : _renderers)
		{
			if (renderer->gameObject->getActive() && renderer->getActive())
			{
				renderer->render();
			}
		}
	}


	Camera* ObjectRenderer::getCurrentCamera()
	{
		return _currentCamera;
	}
}