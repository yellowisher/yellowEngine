#ifndef __H_TEXTURE__
#define __H_TEXTURE__

using namespace std;

#include <string>
#include <map>

class Texture
{
public:
	static Texture* create(const char* path);

	void use();

private:
	static map<string, Texture*> textures;

	unsigned int id;

	Texture();
	~Texture();
};

#endif