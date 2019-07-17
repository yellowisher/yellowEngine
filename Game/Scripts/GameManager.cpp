#include "GameManager.hpp"


void GameManager::start()
{
	std::string castleNames[2] = { "Castle", "Enemy Castle" };

	for (int i = 0; i < 2; i++)
	{
		Transform* castle = GameObject::find(castleNames[i])->transform;

		for (auto child : castle->getChildren())
		{
			auto spawner = child->gameObject->getComponent<Spawner>();
			if (spawner != nullptr)
			{
				_spawners[i].push_back(spawner);
			}
		}
	}
}


void GameManager::update()
{
	static int i = 0;
	for (int k = GLFW_KEY_1; k <= GLFW_KEY_6; k++)
	{
		if (InputManager::getKeyDown(k))
		{
			int team = (k <= GLFW_KEY_3) ? 0 : 1;
			int sp = k - GLFW_KEY_1 - team * 3;

			_spawners[team][sp]->spawn((Unit::UnitType)(i % 8));
			i++;
		}
	}

	UIRenderer::drawImage(Texture::create("./res/Texture/folder.png"), Vector2(100, 100), Vector2(300, 300));
}