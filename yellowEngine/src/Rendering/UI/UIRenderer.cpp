#include "glad/glad.h"
#include <vector>

#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Rendering/UI/UIRenderer.hpp"


namespace yellowEngine
{
	std::vector<UIRenderer*> UIRenderer::_renderers;

	void UIRenderer::renderAll()
	{
		for (auto renderer : _renderers)
		{
			renderer->render();
		}
	}


	UIRenderer::UIRenderer()
	{
		_renderers.push_back(this);
	}


	UIRenderer::~UIRenderer()
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
}