#include "Renderer.hpp"
#include "GameObject.hpp"


std::list<Renderer*> Renderer::_renderers;
Camera* Renderer::_currentCamera;


Renderer::Renderer(GameObject* gameObject) :Component(gameObject)
{
	_renderers.push_back(this);
}


Renderer::~Renderer()
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


void Renderer::render()
{
	_render();
}


void Renderer::renderAll(Camera* camera)
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
