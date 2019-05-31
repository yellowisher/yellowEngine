#include <gl/glew.h>

#include "VertexLayout.hpp"


VertexLayout::Attribute::Attribute(Type type, int size) :type(type), size(size)
{
}


VertexLayout::VertexLayout(std::vector<Attribute> attributes) : _attributes(attributes)
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


int VertexLayout::getAttrCount() const
{
	return _attributes.size();
}


const VertexLayout::Attribute VertexLayout::getAttr(int index) const
{
	// assert?
	return _attributes[index];
}


void VertexLayout::bind() const
{
	unsigned int offset = 0;
	for (int i = 0; i < getAttrCount(); i++)
	{
		auto attr = getAttr(i);
		glVertexAttribPointer(i, attr.size, GL_FLOAT, GL_FALSE, _vertexSize, (void*)offset);
		offset += sizeof(float) * attr.size;
		glEnableVertexAttribArray(i);
	}
}