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
		Technique_Deferred();
		~Technique_Deferred();

	private:
		virtual void _renderScene() override;

		void geometryPass();
		void dirLightPass();
		void spotLightStencilPass(const Light* light);
		void spotLightPass(const Light* light);
		void pointStencilPass(const Light* light);
		void pointLightPass(const Light* light);
		void lightPassBase(Light::LightType type);

		FrameBuffer _geometryBuffer;
		const char* _geometryFsPath;
		Mesh* _meshes[Light::Num_LightType];
		Shader* _lightShaders[Light::Num_LightType];
		Shader* _stencilShader;
	};
}

#endif