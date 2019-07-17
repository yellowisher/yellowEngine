#ifndef __H_VERTEXLAYOUT__
#define __H_VERTEXLAYOUT__

#include <map>
#include <vector>
#include <string>
#include <glad/glad.h>

#include "yellowEngine/Utility/Definitions.hpp"

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
		Attr_Tangent,
		Attr_Bitangent,
		Num_Usage
	};

	class VertexLayout
	{
	public:
		struct Attribute
		{
			Attribute() {};
			Attribute(std::string name, GLenum type, AttributeUsage usage, int size) :
				name(name), type(type), usage(usage), size(size)
			{
			}

			std::string name;
			GLenum type;
			AttributeUsage usage;
			int size;
			size_t offset;
		};

		VertexLayout(std::vector<AttributeUsage> attributes);
		~VertexLayout();

		unsigned int getVertexSize() const;
		size_t getAttrCount() const;
		const std::map<std::string, Attribute>& getAttributes() const;

		// mutable structure is evil!
		const Attribute& getAttr(std::string name) const;

	private:
		struct StaticConstructor
		{
			StaticConstructor()
			{
				__attributeInfo = new Attribute[Num_Usage];
				__attributeInfo[Attr_Position	] = Attribute("a_Position"	, GL_FLOAT, Attr_Position  , 3);
				__attributeInfo[Attr_Normal		] = Attribute("a_Normal"	, GL_FLOAT, Attr_Normal    , 3);
				__attributeInfo[Attr_Color		] = Attribute("a_Color"		, GL_FLOAT, Attr_Color     , 3);
				__attributeInfo[Attr_TexCoord0	] = Attribute("a_TexCoord0"	, GL_FLOAT, Attr_TexCoord0 , 2);
				__attributeInfo[Attr_TexCoord1	] = Attribute("a_TexCoord1"	, GL_FLOAT, Attr_TexCoord1 , 2);
				__attributeInfo[Attr_TexCoord2	] = Attribute("a_TexCoord2"	, GL_FLOAT, Attr_TexCoord2 , 2);
				__attributeInfo[Attr_TexCoord3	] = Attribute("a_TexCoord3"	, GL_FLOAT, Attr_TexCoord3 , 2);
				__attributeInfo[Attr_Joints		] = Attribute("a_Joints"	, GL_FLOAT, Attr_Joints    , MaxJointCount);
				__attributeInfo[Attr_Weights	] = Attribute("a_Weights"	, GL_FLOAT, Attr_Weights   , MaxJointCount);
				__attributeInfo[Attr_Tangent	] = Attribute("a_Tangent"	, GL_FLOAT, Attr_Tangent   , 3);
				__attributeInfo[Attr_Bitangent	] = Attribute("a_Bitangent"	, GL_FLOAT, Attr_Bitangent , 3);

			}
		};

		static StaticConstructor __staticConstructor;
		static Attribute* __attributeInfo;

		std::map<std::string, Attribute> _attributes;
		unsigned int _vertexSize;
	};
}

#endif