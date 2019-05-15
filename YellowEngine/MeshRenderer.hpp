#ifndef __H_MESHRENDERER__
#define __H_MESHRENDERER__

#include "Renderer.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

class MeshRenderer : Renderer
{
public:
	MeshRenderer(Mesh* mesh);
	~MeshRenderer();

	void render();
	void setTexture(Texture* texture);

private:
	Mesh* _mesh;
	Texture* _texture;
};

#endif