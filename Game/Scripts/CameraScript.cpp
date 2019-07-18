#include "CameraScript.hpp"


COMPONENT_IMPL(CameraScript)

bool CameraScript::lockCamera = false;


void CameraScript::start()
{
	_camera = gameObject->getComponent<Camera>();
}


void CameraScript::update()
{
	static float moveSpeed = 0.2f;
	static float boundFactor = 0.1f;
	static float scrollDelta = 0;
	static float scrollSpeed = 0.1f;
	static float minScroll = 0.0001f;
	static Vector2 boundX = Vector2((float)Display::width * boundFactor, (float)Display::width * (1.0f - boundFactor));
	static Vector2 boundY = Vector2((float)Display::height * boundFactor, (float)Display::height * (1.0f - boundFactor));

	Vector3 movement;
	if (!lockCamera)
	{
		Vector2 mousePosition = InputManager::getMousePosition();

		Vector3 move = Vector3::zero;
		if (mousePosition.x < boundX.min) move.x -= 1.0f;
		else if (mousePosition.x > boundX.max) move.x += 1.0f;
		if (mousePosition.y < boundY.min) move.z -= 1.0f;
		else if (mousePosition.y > boundY.max) move.z += 1.0f;

		Vector3 forward = transform->getForward();
		forward.y = 0; forward.normalize();

		Vector3 right = transform->getRight();
		right.y = 0; right.normalize();

		movement += forward * move.z;
		movement += right * move.x;
		movement = movement * moveSpeed;
	}

	float newScroll = InputManager::getMouseScroll();
	if (newScroll != 0)
	{
		scrollDelta = newScroll;
	}
	else if (scrollDelta != 0)
	{
		scrollDelta *= 0.92f;
		if (Utils::abs(scrollDelta) < minScroll) scrollDelta = 0;
	}
	_camera->setZoom(_camera->getZoom() + scrollDelta * scrollSpeed * 500);
	//movement += transform->getForward() * scrollDelta * scrollSpeed;

	transform->translate(movement);
}
