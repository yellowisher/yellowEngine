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
	MeshRenderer(GameObject* gameObject);
	~MeshRenderer();

	void set(Mesh* mesh, ShaderProgram* shader);
	void _render();
	void setTexture(Texture* texture);

private:
	Mesh* _mesh;
	ShaderProgram* _shader;
	VertexLayoutBinding* _binding;
	unsigned int _modelUniformHandle;

	Texture* _texture;
};

#endif