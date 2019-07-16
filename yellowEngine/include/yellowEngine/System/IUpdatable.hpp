#ifndef __H_IUPDATABLE__
#define __H_IUPDATABLE__

namespace yellowEngine
{
	class Game;
	class IUpdatable
	{
		friend class Game;
	public:
		IUpdatable()
		{
			reportCreate(this);
		}
		virtual ~IUpdatable()
		{
			reportDestroy(this);
		}
		virtual void update() {}
		virtual void start() {};

	private:
		static void(*reportCreate)(IUpdatable* updatable);
		static void(*reportDestroy)(IUpdatable* updatable);
	};
}

#endif