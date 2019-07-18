#ifndef __UIRENDERER__
#define __UIRENDERER__

#include <vector>

namespace yellowEngine
{
	class UIRenderer
	{
	public:
		static void renderAll();

		UIRenderer();
		virtual ~UIRenderer();

	protected:
		virtual void render() = 0;

	private:
		static std::vector<UIRenderer*> _renderers;
	};
}

#endif