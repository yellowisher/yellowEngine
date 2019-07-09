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
			onCreate(this);
		}
		virtual ~IUpdatable()
		{
			onDestroy(this);
		}
		virtual void start() = 0;
		virtual void update() = 0;

	private:
		static void(*onCreate)(IUpdatable* updatable);
		static void(*onDestroy)(IUpdatable* updatable);
	};
}

#endif