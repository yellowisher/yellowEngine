#ifndef __H_MESHRENDERER__
#define __H_MESHRENDERER__

#include "Renderer.hpp"
#include "Mesh.hpp"

class MeshRenderer : Renderer
{
public:
	MeshRenderer(Mesh* mesh);
	~MeshRenderer();

	void render();

private:
	Mesh* mesh;
};

#endif