#ifndef __H_VERTEXLAYOUT__
#define __H_VERTEXLAYOUT__

#include <vector>

class VertexLayout
{
public:
	enum Type
	{
		POSITION,
		NORMAL,
		COLOR,
		TEXCOORD0,
	};

	class Attribute
	{
	public:
		Type type;
		unsigned int size;

		Attribute(Type type, int size);
	};

	VertexLayout(std::vector<Attribute> attributes);
	~VertexLayout();

	unsigned int getVertexSize() const;
	int getAttrCount() const;
	// mutable structure is always evil!
	const Attribute getAttr(int index) const;

private:
	std::vector<Attribute> _attributes;
	unsigned int _vertexSize;
};

#endif