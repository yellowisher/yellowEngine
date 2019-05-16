#ifndef __H_MESHRENDERER__
#define __H_MESHRENDERER__

#include "Component.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "VertexLayoutBinding.hpp"
#include "Texture.hpp"

class MeshRenderer : public Renderer
{
public:
	void render();
	void setTexture(Texture* texture);

	//protected
public:
	MeshRenderer(Mesh* mesh, ShaderProgram* shader);
	~MeshRenderer();

private:
	Mesh* _mesh;
	ShaderProgram* _shader;
	VertexLayoutBinding* _binding;

	Texture* _texture;
};

#endif