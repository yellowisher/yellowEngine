#ifndef __H_VERTEXLAYOUT__
#define __H_VERTEXLAYOUT__

#include <vector>
#include <string>

#include "yellowEngine/Rendering/Mesh.hpp"

namespace yellowEngine
{
	enum AttributeUsage
	{
		Attr_Position,
		Attr_Normal,
		Attr_Color,
		Attr_TexCoord0,
		Attr_TexCoord1,
		Attr_TexCoord2,
		Attr_TexCoord3,
		Attr_Joints,
		Attr_Weights,
		Num_Usage
	};

	class VertexLayout
	{
	public:
		struct Attribute
		{
			Attribute(std::string name, GLenum type, int size) :
				name(name), type(type), size(size)
			{
			}

			std::string name;
			GLenum type;
			int size;
			int offset;
		};

		VertexLayout(std::vector<AttributeUsage> attributes);
		~VertexLayout();

		unsigned int getVertexSize() const;
		size_t getAttrCount() const;
		// mutable structure is evil!
		const Attribute& getAttr(std::string name) const;

	private:
		struct StaticConstructor
		{
			StaticConstructor()
			{
				__attributeInfo[Usage_Position	] = AttributeInfo("a_Position"	, GL_FLOAT, 3);
				__attributeInfo[Usage_Normal	] = AttributeInfo("a_Normal"	, GL_FLOAT, 3);
				__attributeInfo[Usage_Color		] = AttributeInfo("a_Color"		, GL_FLOAT, 3);
				__attributeInfo[Usage_TexCoord0	] = AttributeInfo("a_TexCoord0"	, GL_FLOAT, 2);
				__attributeInfo[Usage_TexCoord1	] = AttributeInfo("a_TexCoord1"	, GL_FLOAT, 2);
				__attributeInfo[Usage_TexCoord2	] = AttributeInfo("a_TexCoord2"	, GL_FLOAT, 2);
				__attributeInfo[Usage_TexCoord3	] = AttributeInfo("a_TexCoord3"	, GL_FLOAT, 2);
				__attributeInfo[Usage_Joints	] = AttributeInfo("a_Joints"	, GL_FLOAT, Mesh::MaxJointCount);
				__attributeInfo[Usage_Weights	] = AttributeInfo("a_Weights"	, GL_FLOAT, Mesh::MaxJointCount);
			}
		};

		static StaticConstructor __staticConstructor;
		static Attribute __attributeInfo[Num_Usage];

		std::map<std::string, Attribute> _attributes;
		unsigned int _vertexSize;
	};
}

#endif