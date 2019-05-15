#ifndef __H_VERTEXLAYOUT__
#define __H_VERTEXLAYOUT__

using namespace std;

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

	VertexLayout(vector<Attribute> attributes);
	~VertexLayout();

	unsigned int getVertexSize() const;
	unsigned int getAttrCount() const;
	// mutable structure is always evil!
	const Attribute getAttr(int index) const;

private:
	vector<Attribute> _attributes;
	unsigned int _vertexSize;
};

#endif