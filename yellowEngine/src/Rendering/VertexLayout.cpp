#include <glad/glad.h>

#include "yellowEngine/Rendering/VertexLayout.hpp"


namespace yellowEngine
{
	VertexLayout::StaticConstructor VertexLayout::__staticConstructor;
	VertexLayout::Attribute* VertexLayout::__attributeInfo;

	VertexLayout::VertexLayout(std::vector<AttributeUsage> attributes)
	{
		size_t offset = 0;
		for (auto usage : attributes)
		{
			Attribute attr = __attributeInfo[usage];
			attr.offset = offset;
			_attributes.insert({ attr.name, attr });
			offset += sizeof(float) * attr.size;
		}
		_vertexSize = (unsigned int)offset;
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


	const std::map<std::string, VertexLayout::Attribute>& VertexLayout::getAttributes()
	{
		return _attributes;
	}


	const VertexLayout::Attribute& VertexLayout::getAttr(std::string name) const
	{
		// assert?
		return _attributes.at(name);
	}
}