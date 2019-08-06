#ifndef __H_CAMERASCRIPT__
#define __H_CAMERASCRIPT__

#include <yellowEngine/yellowEngine.hpp>

using namespace yellowEngine;

class CameraScript : public Component, public IUpdatable
{
	BEGIN_COMPONENT(CameraScript)
	END_COMPONENT

public:
	static void win();
	static bool lockCamera;

	void start() override;
	void update() override;

private:
	Camera* _camera;
};

#endif