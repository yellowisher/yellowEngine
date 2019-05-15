#include "VertexLayout.hpp"

VertexLayout::Attribute::Attribute(Type type, int size) :type(type), size(size)
{
}


VertexLayout::VertexLayout(vector<Attribute> attributes) : _attributes(attributes)
{
	_vertexSize = 0;
	for (auto attribute : attributes)
	{
		_vertexSize += sizeof(float) * attribute.size;
	}
}


VertexLayout::~VertexLayout()
{
}


unsigned int VertexLayout::getVertexSize() const
{
	return _vertexSize;
}


unsigned int VertexLayout::getAttrCount() const
{
	return _attributes.size();
}


const VertexLayout::Attribute VertexLayout::getAttr(int index) const
{
	// assert?
	return _attributes[index];
}