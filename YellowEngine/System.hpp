#ifndef __H_SYSTEM__
#define __H_SYSTEM__

class System
{
public:
	static System* getInstance();

	float getWidth();
	float getHeight();
	void setWidth(float width);
	void setHeight(float height);
	float getAspectRatio();

private:
	static System* _instance;

	float _width;
	float _height;

	System();
	System(const System& copy);
	~System();
};

#endif