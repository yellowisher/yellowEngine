#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Component/Light.hpp"
#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/Technique/Technique_Deferred.hpp"


namespace yellowEngine
{
	static constexpr int GeometryColorBegin = 0;
	static constexpr int GeometryColorCount = 3;

	enum
	{
		HDR_Scene,
		HDR_Bloom
	};
	enum
	{
		Blur_,
		Blur_Result
	};

	static constexpr int BloomBlurCount = 10;

	bool Technique_Deferred::applyBloom = true;
	float Technique_Deferred::exposure = 1.0;

	Technique_Deferred::Technique_Deferred()
	{
		_shadowMappingShader = Shader::create("./res/Shader/shadow.vert", "./res/Shader/shadow.frag");
		_pointShadowMappingShader = Shader::create("./res/Shader/shadow_point.vert", "./res/Shader/shadow_point.frag");

		// deferred shading frame buffer
		_geometryBuffer.addColorAttachment("u_PositionMap", GL_RGB32F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_geometryBuffer.addColorAttachment("u_NormalMap", GL_RGB32F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_geometryBuffer.addColorAttachment("u_ColorMap", GL_RGBA, Display::width, Display::height, GL_RGBA, GL_UNSIGNED_BYTE);
		_geometryBuffer.addDepthStencilAttachment(Display::width, Display::height);
		_geometryBuffer.init();

		_hdrBuffer.addColorAttachment("ColorBuffer", GL_RGB16F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_hdrBuffer.addColorAttachment("BrightBuffer", GL_RGB16F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_hdrBuffer.init();

		_blurBuffer.addColorAttachment("First", GL_RGB16F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_blurBuffer.addColorAttachment("Second", GL_RGB16F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_blurBuffer.init();

		_geometryFsPath = "./res/Shader/deferred_geometry.frag";

		_meshes[Light::LightType_Dir] = Mesh::create("./res/Mesh/quad.obj");
		_meshes[Light::LightType_Spot] = Mesh::create("./res/Mesh/cone.obj");
		_meshes[Light::LightType_Point] = Mesh::create("./res/Mesh/sphere.obj");

		_lightShaders[Light::LightType_Dir] = Shader::create("./res/Shader/deferred_light.vert", "./res/Shader/deferred_light_dir.frag");
		_lightShaders[Light::LightType_Spot] = Shader::create("./res/Shader/deferred_light.vert", "./res/Shader/deferred_light_spot.frag");
		_lightShaders[Light::LightType_Point] = Shader::create("./res/Shader/deferred_light.vert", "./res/Shader/deferred_light_point.frag");
		_nullShader = Shader::create("./res/Shader/null.vert", "./res/Shader/null.frag");
		_blurShader = Shader::create("./res/Shader/bloom_blur.vert", "./res/Shader/bloom_blur.frag");
		_bloomShader = Shader::create("./res/Shader/bloom.vert", "./res/Shader/bloom.frag");
	}


	Technique_Deferred::~Technique_Deferred()
	{
	}


	void Technique_Deferred::_renderScene(Camera* camera)
	{
		Camera::currentCamera = camera;

		_hdrBuffer.bindForDrawing();
		glClear(GL_COLOR_BUFFER_BIT);

		// shadow mapping pass
		glCullFace(GL_FRONT);
		for (int type = 0; type < Light::Num_LightType; type++)
		{
			for (auto light : Light::getLights((Light::LightType)type))
			{
				if(light->castShadow) shadowMappingPass(light);
			}
		}
		glCullFace(GL_BACK);

		// restore view port
		FrameBuffer::unbind();
		glViewport(0, 0, Display::width, Display::height);

		geometryPass();

		// light passes
		glDepthMask(GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		for (auto light : Light::getLights(Light::LightType_Point))
		{
			stencilPass(light);
			lightPass(light);
		}

		for (auto light : Light::getLights(Light::LightType_Spot))
		{
			stencilPass(light);
			lightPass(light);
		}
		glDisable(GL_STENCIL_TEST);

		for (auto light : Light::getLights(Light::LightType_Dir))
		{
			lightPass(light);
		}

		if (!applyBloom)
		{
			glDepthMask(GL_TRUE);
			return;
		}

		_geometryBuffer.unbind();

		// blurding
		int horizontal = 0;
		_blurShader->bind();

		_blurBuffer.bindForDrawing();
		for (int i = 0; i < BloomBlurCount; i++)
		{
			// actually horizontal and vertical are reversed
			_blurShader->setUniform("u_Horizontal", horizontal);
			_blurBuffer.setDrawBuffer(horizontal);

			if (i == 0)
			{
				glActiveTexture(GL_TEXTURE0);
				_hdrBuffer.getColorTexture(HDR_Bloom)->bind();
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				_blurBuffer.getColorTexture(1 - horizontal)->bind();
			}

			renderQuad(_blurShader);
			horizontal = 1 - horizontal;
		}

		// blending with tone mapping and gamma correction
		_bloomShader->bind();
		_bloomShader->setUniform("u_ApplyBloom", applyBloom);
		_bloomShader->setUniform("u_Exposure", exposure);

		int sceneUnit = _bloomShader->getTextureUnit("u_Scene");
		glActiveTexture(GL_TEXTURE0 + sceneUnit);
		_hdrBuffer.getColorTexture(HDR_Scene)->bind();

		int bloomUnit = _bloomShader->getTextureUnit("u_Bloom");
		glActiveTexture(GL_TEXTURE0 + bloomUnit);
		_blurBuffer.getColorTexture(Blur_Result)->bind();

		FrameBuffer::unbind();
		renderQuad(_bloomShader);

		glDepthMask(GL_TRUE);
	}


	void Technique_Deferred::shadowMappingPass(Light* light)
	{
		static std::pair< Vector3, Vector3> dirUpPairs[] = {
			{Vector3( 1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)},
			{Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)},
			{Vector3( 0.0f,  1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f)},
			{Vector3( 0.0f, -1.0f,  0.0f), Vector3(0.0f,  0.0f,  -1.0f)},
			{Vector3( 0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)},
			{Vector3( 0.0f,  0.0f, -1.0f), Vector3(0.0f, -1.0f,  0.0f)}
		};

		auto shadowBuffer = light->getShadowBuffer();
		shadowBuffer->bindForDrawing();
		glViewport(0, 0, shadowBuffer->getDepthMapWidth(), shadowBuffer->getDepthMapHeight());

		if (light->getType() == Light::LightType_Point)
		{
			glCullFace(GL_BACK);

			_pointShadowMappingShader->bind();
			Vector3 eye = light->transform->getWorldPosition();

			glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
			
			for (unsigned int i = 0; i < 6; i++)
			{
				Matrix projection = light->getProjMatrix();
				Matrix view = Matrix::lookAtDir(eye, dirUpPairs[i].first, dirUpPairs[i].second);
				
				_pointShadowMappingShader->setUniform("u_LightPosition", light->transform->getWorldPosition());
				_pointShadowMappingShader->setUniform("u_LightProjView", projection * view);

				GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
				glFramebufferTexture2D(
					GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face,
					light->getShadowBuffer()->getDepthMapId(), 0);
				glDrawBuffer(GL_COLOR_ATTACHMENT0);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				ObjectRenderer::renderAll("./res/Shader/shadow_point.vert", "./res/Shader/shadow_point.frag");
			}
			glClearColor(0, 0, 0, 0);
			glCullFace(GL_FRONT);
		}
		else
		{
			_shadowMappingShader->bind();
			_shadowMappingShader->setUniform("u_LightProjView", light->getProjViewMatrix());

			glClear(GL_DEPTH_BUFFER_BIT);
			ObjectRenderer::renderAll("./res/Shader/shadow.vert", "./res/Shader/shadow.frag");
		}
	}


	void Technique_Deferred::geometryPass()
	{
		glDisable(GL_CULL_FACE);
		_geometryBuffer.bindForDrawing();
		_geometryBuffer.setDrawBuffer(GeometryColorBegin, GeometryColorCount);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ObjectRenderer::renderAll(nullptr, _geometryFsPath);
		glEnable(GL_CULL_FACE);
	}


	void Technique_Deferred::stencilPass(Light* light)
	{
		_geometryBuffer.setDrawBuffer(-1);

		glStencilMask(GL_TRUE);
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		_nullShader->bind();

		auto type = light->getType();
		float range = light->getRange();
		Vector3 scale;
		if (type == Light::LightType_Spot)
		{
			float cos2 = light->getOuterCutoffCos() * light->getOuterCutoffCos();
			float sin2 = 1.0f - cos2;

			scale.x = scale.y = range * sqrtf(sin2 / cos2);
			scale.z = range / 2.0f;
		}
		else
		{
			scale.x = scale.y = scale.z = range;
		}

		Matrix pv = Camera::currentCamera->getMatrix();
		Matrix pvw = pv * light->transform->getMatrix() * Matrix::createScale(scale);

		_nullShader->setUniform("u_ProjViewWorld", pvw);

		VertexLayoutBinding::create(_meshes[type], _nullShader)->bind();
		glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);

		glStencilMask(GL_FALSE);
		glEnable(GL_CULL_FACE);
	}


	void Technique_Deferred::lightPass(Light* light)
	{
		if (applyBloom)
		{
			_hdrBuffer.bindForDrawing();
			_hdrBuffer.setDrawBuffer(0, 2);
		}
		else
		{
			FrameBuffer::unbind();
		}

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		auto type = light->getType();
		_lightShaders[type]->bind();
		light->updateUniforms(_lightShaders[type]);

		int i = 0;
		const auto& textureNames = _lightShaders[type]->getTextureUnits();
		const auto& colorBuffers = _geometryBuffer.getColorBuffers();
		for (; i < textureNames.size(); i++)
		{
			for (auto it = colorBuffers.begin(); it != colorBuffers.end(); ++it)
			{
				if (it->first == textureNames[i])
				{
					glActiveTexture(GL_TEXTURE0 + i);
					it->second->bind();
					break;
				}
			}
		}

		if (light->castShadow)
		{
			glActiveTexture(GL_TEXTURE0 + i - 1);
			light->getShadowBuffer()->bindDepthTexture();
		}

		VertexLayoutBinding::create(_meshes[type], _lightShaders[type])->bind();

		if (light->getType() != Light::LightType_Dir)
		{
			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
			glCullFace(GL_FRONT);
		}

		glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);

		glCullFace(GL_BACK);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}