#ifndef __H_VERTEXLAYOUT__
#define __H_VERTEXLAYOUT__

#include <vector>

class VertexLayout
{
public:
	enum Type
	{
		Attr_Position,
		Attr_Normal,
		Attr_Color,
		Attr_TexCoord0,
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
	size_t getAttrCount() const;
	// mutable structure is always evil!
	const Attribute getAttr(int index) const;
	void bind() const;

private:
	std::vector<Attribute> _attributes;
	unsigned int _vertexSize;
};

#endif