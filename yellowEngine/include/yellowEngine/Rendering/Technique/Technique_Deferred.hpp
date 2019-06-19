#ifndef __H_TECHNIQUETYPE_DEFERRED__
#define __H_TECHNIQUETYPE_DEFERRED__

#include <string>

#include "yellowEngine/Rendering/FrameBuffer.hpp"
#include "yellowEngine/Rendering/Technique.hpp"

namespace yellowEngine
{
	class Technique_Deferred : public Technique
	{
	public:
		Technique_Deferred();
		~Technique_Deferred();

	private:
		virtual void _renderScene() override;

		FrameBuffer _geometryBuffer;
		const char* _geometryFsPath;
	};
}

#endif