#ifndef __H_MESHRENDERER__
#define __H_MESHRENDERER__

#include "Component.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

class MeshRenderer : public Renderer
{
public:
	void render();
	void setTexture(Texture* texture);

protected:
	MeshRenderer(Mesh* mesh);
	~MeshRenderer();

private:
	Mesh* _mesh;
	Texture* _texture;
};

#endif