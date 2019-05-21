#ifndef __H_SYSTEM__
#define __H_SYSTEM__

class System
{
public:
	static System* getInstance();

	int getWidth();
	int getHeight();
	void setWidth(int width);
	void setHeight(int height);
	float getAspectRatio();

private:
	static System* _instance;

	int _width;
	int _height;

	System();
	System(const System& copy);
	~System();
};

#endif