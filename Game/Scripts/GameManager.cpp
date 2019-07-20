#include "GameManager.hpp"


void GameManager::start()
{
	// create UIs
	static Vector2 margin = Vector2(32, 32);
	static Vector2 imageSize = Vector2(96, 96);
	static Vector2 imageGap = Vector2(16, 16);
	static Texture* icons[] = {
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\sword.png",			true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\spear.png",			true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\bow.png",			true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\crossbow.png",		true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\warhammer.png",		true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\cavalry_hammer.png", true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\cavalry_sword.png",	true),
		Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\icon\\cavalry_lance.png",	true)
	};
	Texture* panel = Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\panel.png", true);
	barTexture = Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\bar.png", true);
	Texture* frame = Texture::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Texture\\bar_frame.png", true);

	for (int i = 0; i < Unit::Num_Units; i++)
	{
		int base = Unit::getBaseType((Unit::UnitType)i);
		int r = base;
		int c = i - base * Unit::Unit_Cavalry_Start;
		int x = margin.x + imageSize.x / 2 + c * (imageSize.x + imageGap.x);
		int y = margin.y + imageSize.y / 2 + r * (imageSize.y + imageGap.y);
		_images[i] = new Image(Vector2(x, y), imageSize, icons[i]);
	}
	barOrigin = Vector2(margin.x, margin.y + imageGap.y * 2.0f + imageSize.y * 2.0f);

	new Image(frame, barOrigin);
	goldBar = new Image(barTexture, barOrigin);
	goldBar->color = Vector3(1.0f, 0.7f, 0.0f);
	new Image(barTexture, barOrigin);

	new Image(Vector2(panel->width / 2, panel->height / 2), Vector2(panel->width, panel->height), panel);

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


static Vector3 getPlacePosition()
{
	Vector2 pos = InputManager::getMousePosition();
	Ray ray = Ray::screenPointToRay(Camera::getMainCamera(), pos.x, pos.y);
	Vector3 point = ray.pointOnRay(1, 0);
	return point;
}


void GameManager::update()
{
	static int poss = 2;
	static int maxGold = 1000;

	static int costs[Unit::Num_Units] = {
		120, 120, 200, 250, 300,
		350, 400, 500
	};
	static int buttons[Unit::Num_Units] = {
		GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_G,
		GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E
	};

	static int i = 0;
	for (int k = GLFW_KEY_1; k <= GLFW_KEY_6; k++)
	{
		if (InputManager::getKeyDown(k))
		{
			int team = 1;
			int sp = (k - GLFW_KEY_1) % 3;

			_spawners[team][sp]->spawn((Unit::UnitType)(i % 8));
			i++;
		}
	}

	static int f = 0;
	static int pos[] = {
		0,
		2,
		2,

		1,
		1,

		0,
		2,
		0
	};
	static std::vector<std::pair<int, Unit::UnitType>> po = {
		{30+500, Unit::Unit_Sword},
		{700 + 500, Unit::Unit_Spear},
		{1400 + 330, Unit::Unit_Armor},

		{1400 + 500, Unit::Unit_Cavalry_Sword},
		{2200, Unit::Unit_Spear},

		{2000+500, Unit::Unit_Armor},
		{2020+500, Unit::Unit_Sword},
		{2050+500, Unit::Unit_Bow},
	};

	++f;
	for (int i = 0; i < 6; i++)
	{
		auto p = po[i];
		if (f == p.first)
		{
			_spawners[1][pos[i]]->spawn((Unit::UnitType)(p.second));
		}
	}

	if (placing != nullptr)
	{
		Vector3 point = getPlacePosition();
		placing->setPosition(point);
		if (InputManager::getMouseButtonDown(0))
		{
			// place real unit
			delete(placing->gameObject);
			placing = nullptr;
			gold -= costs[selectedType];
			Spawner::spawn(selectedType, point, Quaternion(Vector3(0, 90.0f, 0)), 0);
		}
		else
		{
			int possible = (point.x > -4 && point.x <= 3) ? 2 : 3;
			if (possible != poss)
			{
				delete(placing->gameObject);
				poss = possible;
				placing = Spawner::spawn(selectedType, getPlacePosition(), Quaternion(Vector3(0, 90.0f, 0)), possible)->transform;
			}
		}

	}

	int pressed = -1;
	if (++gold > maxGold) gold = maxGold;
	for (auto i = 0; i < Unit::Num_Units; i++)
	{
		if (placing && i == selectedType)
		{
			_images[i]->color = Vector3(0.803f, 1.0f, 0.682f);
		}
		else if (gold > costs[i])
		{
			_images[i]->color = Vector3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			_images[i]->color = Vector3(0.5f, 0.5f, 0.5f);
		}
		
		if (InputManager::getKeyDown(buttons[i]) && gold > costs[i])
		{
			pressed = i;
		}
	}

	float ratio = (float)gold / (float)maxGold;
	goldBar->setBound(barOrigin, barOrigin + Vector2(barTexture->width * ratio, barTexture->height));

	if (pressed != -1)
	{
		bool same = false;
		if (placing != nullptr)
		{
			delete(placing->gameObject);
			placing = nullptr;

			if (pressed == selectedType)
			{
				same = true;
			}
		}

		if (!same)
		{
			Vector3 point = getPlacePosition();
			selectedType = (Unit::UnitType)pressed;
			placing = Spawner::spawn(selectedType, getPlacePosition(), Quaternion(Vector3(0, 90.0f, 0)), poss)->transform;
		}
	}
}