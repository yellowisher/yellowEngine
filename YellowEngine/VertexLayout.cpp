#include "VertexLayout.hpp"

VertexLayout::Attribute::Attribute(Type type, int size) :type(type), size(size)
{
}


VertexLayout::VertexLayout(vector<Attribute> attributes) : attributes(attributes)
{
	vertexSize = 0;
	for (auto attribute : attributes)
	{
		vertexSize += sizeof(float) * attribute.size;
	}
}


VertexLayout::~VertexLayout()
{
}


unsigned int VertexLayout::getVertexSize() const
{
	return vertexSize;
}


unsigned int VertexLayout::getAttrCount() const
{
	return attributes.size();
}


const VertexLayout::Attribute VertexLayout::getAttr(int index) const
{
	// assert?
	return attributes[index];
}