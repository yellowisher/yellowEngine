#include <glad/glad.h>

#include "yellowEngine/Rendering/VertexLayout.hpp"


namespace yellowEngine
{
	VertexLayout::StaticConstructor VertexLayout::__staticConstructor;
	VertexLayout::Attribute VertexLayout::__attributeInfo[Num_Usage];


	VertexLayout::VertexLayout(std::vector<AttributeUsage> attributes)
	{
		int offset = 0;
		for (auto usage : attributes)
		{
			Attribute attr = __attributeInfo[usage];
			attr.offset = offset;
			_attributes.insert({ attr.name, attr });
			offset += sizeof(float) * attr.size;
		}
		_vertexSize = offset;
	}


	VertexLayout::~VertexLayout()
	{
	}


	unsigned int VertexLayout::getVertexSize() const
	{
		return _vertexSize;
	}


	size_t VertexLayout::getAttrCount() const
	{
		return _attributes.size();
	}


	const VertexLayout::Attribute& VertexLayout::getAttr(std::string name) const
	{
		// assert?
		return _attributes[name];
	}
}