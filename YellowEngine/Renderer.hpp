#ifndef __H_RENDERER__
#define __H_RENDERER__

#include "AABB.hpp"
#include "Vector3.hpp"
#include "GameObject.hpp"
#include "ShaderProgram.hpp"
#include "VertexLayout.hpp"

// non-component generic renderer

class Renderer
{
public:
	enum Type
	{
		Type_Box,
		Type_Sphere,
		Num_Type
	};

	struct Box
	{
		Box(Vector3 min, Vector3 max) :min(min), max(max) {}
		~Box() {}

		static const int count = 24;
		union
		{
			struct
			{
				Vector3 min;
				Vector3 max;
			};
			Vector3 m[2];
		};
	};

	struct Sphere
	{
		static const int count = 0;

	};

	Renderer();
	Renderer(VertexLayout& layout, ShaderProgram* shader, GameObject* gameObject);
	~Renderer();

	void setData(Box&& box);
	void setData(AABB aabb);

	void setData(Sphere& sphere);
	void render();

private:
	inline int getCount(Type type){
		switch (type)
		{
			case Type_Box:
				return Box::count;
			case Type_Sphere:
				return Sphere::count;
			default:
				return 0;
		}
	}

	Type _type;
	unsigned int _vbo;
	unsigned int _vao;

	GameObject* _gameObject;
	ShaderProgram* _shader;
};

#endif