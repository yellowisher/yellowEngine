#include "GameManager.hpp"


void GameManager::start()
{
	// create UIs
	static Vector2 margin = Vector2(32, 32);
	static Vector2 imageSize = Vector2(96, 96);
	static Vector2 imageGap = Vector2(16, 16);
	static Texture* icons[] = {
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\sword.png",			GL_REPEAT, GL_NEAREST, true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\spear.png",			GL_REPEAT, GL_NEAREST, true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\bow.png",			GL_REPEAT, GL_NEAREST, true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\crossbow.png",		GL_REPEAT, GL_NEAREST, true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\warhammer.png",		GL_REPEAT, GL_NEAREST, true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\cavalry_hammer.png", GL_REPEAT, GL_NEAREST, true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\cavalry_sword.png",	GL_REPEAT, GL_NEAREST, true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\cavalry_lance.png",	GL_REPEAT, GL_NEAREST, true)
	};

	Texture* panel = Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\panel.png");
	new Image(Vector2(panel->width / 2, panel->height / 2), Vector2(panel->width, panel->height), panel);

	for (int i = 0; i < Unit::Num_Units; i++)
	{
		int base = Unit::getBaseType((Unit::UnitType)i);
		int r = base;
		int c = i - base * Unit::Unit_Cavalry_Start;
		int x = margin.x + imageSize.x / 2 + c * (imageSize.x + imageGap.x);
		int y = margin.y + imageSize.y / 2 + r * (imageSize.y + imageGap.y);
		_images[i] = new Image(Vector2(x, y), imageSize, icons[i]);
	}

	gold = 50;
	placing = nullptr;

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
	static int costs[Unit::Num_Units] = {
		120, 120, 200, 250, 300,
		350, 400, 450
	};

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

	if (placing != nullptr)
	{
		Vector2 pos = InputManager::getMousePosition();
		Ray ray = Ray::screenPointToRay(Camera::getMainCamera(), pos.x, pos.y);
		Vector3 point = ray.pointOnRay(1, 0.5f);
	}

	gold++;
	for (auto i = 0; i < Unit::Num_Units; i++)
	{
		if (gold > costs[i])
		{
			_images[i]->color = Vector3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			_images[i]->color = Vector3(0.7f, 0.7f, 0.7f);
		}
		
	}

}