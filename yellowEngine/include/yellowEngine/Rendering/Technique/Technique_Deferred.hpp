#ifndef __H_TECHNIQUETYPE_DEFERRED__
#define __H_TECHNIQUETYPE_DEFERRED__

#include <string>

#include "yellowEngine/Component/Light.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"
#include "yellowEngine/Rendering/FrameBuffer.hpp"
#include "yellowEngine/Rendering/Technique.hpp"

namespace yellowEngine
{
	class Technique_Deferred : public Technique
	{
	public:
		static bool applyBloom;
		static float exposure;

		Technique_Deferred();
		~Technique_Deferred();

	private:
		virtual void _renderScene(Camera* camera) override;

		void shadowMappingPass(Light* light);
		void geometryPass();

		void stencilPass(Light* light);
		void lightPass(Light* light);

		FrameBuffer _geometryBuffer;
		FrameBuffer _hdrBuffer;
		FrameBuffer _blurBuffer;

		const char* _geometryFsPath;
		Mesh* _meshes[Light::Num_LightType];
		Shader* _lightShaders[Light::Num_LightType];
		Shader* _shadowMappingShader;
		Shader* _pointShadowMappingShader;
		Shader* _blurShader;
		Shader* _nullShader;
		Shader* _bloomShader;
	};
}

#endif