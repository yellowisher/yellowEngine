#ifndef __UIRENDERER__
#define __UIRENDERER__

#include "yellowEngine/Math/Vector2.hpp"
#include "yellowEngine/Rendering/Texture.hpp"
#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"

namespace yellowEngine
{
	class UIRenderer
	{
	public:
		static void drawImage(Texture* image, Vector2 min, Vector2 max);
		static void render();
	};
}

#endif