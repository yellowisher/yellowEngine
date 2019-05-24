#ifndef __H_MESHRENDERER__
#define __H_MESHRENDERER__

#include <vector>

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

	MeshRenderer* set(Mesh* mesh, ShaderProgram* shader);
	void _render();
	void addTexture(Texture* texture, const char* usage);

private:
	Mesh* _mesh;
	ShaderProgram* _shader;
	VertexLayoutBinding* _binding;
	std::vector<Texture*> _textures;
};

#endif