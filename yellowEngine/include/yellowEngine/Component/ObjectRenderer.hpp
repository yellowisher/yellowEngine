#ifndef __H_OBJECTRENDERER__
#define __H_OBJECTRENDERER__

#include <list>

#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Component/Camera.hpp"

namespace yellowEngine
{
	class ObjectRenderer : public Component
	{
	public:
		static void renderAll(Camera* camera, const char* vsPath = nullptr, const char* fsPath = nullptr);
		static Camera* getCurrentCamera();

		ObjectRenderer(GameObject* gameObject);
		virtual ~ObjectRenderer();

		void render(const char* vsPath = nullptr, const char* fsPath = nullptr);

		static Camera* _currentCamera;
	protected:

		virtual void _render(const char* vsPath = nullptr, const char* fsPath = nullptr) = 0;

	private:
		static std::list<ObjectRenderer*> _renderers;

	};
}

#endif