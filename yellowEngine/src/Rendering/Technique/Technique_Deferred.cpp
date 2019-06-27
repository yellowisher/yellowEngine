#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Component/Light.hpp"
#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/Technique/Technique_Deferred.hpp"


namespace yellowEngine
{
	static constexpr float MinLightContrib = 5.0f;
	static constexpr int Final = 3;
	static constexpr int RenderTargetBegin = 0;
	static constexpr int RenderTargetCount = 3;

	Technique_Deferred::Technique_Deferred()
	{
		_geometryBuffer.addColorAttachment("u_PositionMap", GL_RGB32F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_geometryBuffer.addColorAttachment("u_NormalMap", GL_RGB32F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_geometryBuffer.addColorAttachment("u_ColorMap", GL_RGBA, Display::width, Display::height, GL_RGBA, GL_UNSIGNED_BYTE);
		_geometryBuffer.addColorAttachment("Final", GL_RGBA, Display::width, Display::height, GL_RGBA, GL_UNSIGNED_BYTE);
		_geometryBuffer.addDepthStencilAttachment(Display::width, Display::height);
		_geometryBuffer.init();

		_geometryFsPath = "Shader/deferred_geometry.frag";

		_meshes[Light::LightType_Dir  ] = Mesh::create("Mesh/quad.obj");
		_meshes[Light::LightType_Spot ] = Mesh::create("Mesh/cone.obj");
		_meshes[Light::LightType_Point] = Mesh::create("Mesh/sphere.obj");

		_lightShaders[Light::LightType_Dir  ] = Shader::create("Shader/deferred_light.vert", "Shader/deferred_light_dir.frag");
		_lightShaders[Light::LightType_Spot ] = Shader::create("Shader/deferred_light.vert", "Shader/deferred_light_spot.frag");
		_lightShaders[Light::LightType_Point] = Shader::create("Shader/deferred_light.vert", "Shader/deferred_light_point.frag");
		_stencilShader = Shader::create("Shader/null.vert", "Shader/null.frag");
	}


	Technique_Deferred::~Technique_Deferred()
	{
	}


	void Technique_Deferred::_renderScene(Camera* camera)
	{
		Camera::currentCamera = camera;

		_geometryBuffer.bindForDrawing();
		_geometryBuffer.setDrawBuffer(Final);
		glClear(GL_COLOR_BUFFER_BIT);

		_geometryBuffer.setDrawBuffer(RenderTargetBegin, RenderTargetCount);
		geometryPass();

		glEnable(GL_STENCIL_TEST);
		for (auto light : Light::getLights(Light::LightType_Point))
		{
			_geometryBuffer.setDrawBuffer(-1);
			pointStencilPass(light);
			_geometryBuffer.setDrawBuffer(Final);
			pointLightPass(light);
		}

		for (auto light : Light::getLights(Light::LightType_Spot))
		{
			_geometryBuffer.setDrawBuffer(-1);
			spotLightStencilPass(light);
			_geometryBuffer.setDrawBuffer(Final);
			spotLightPass(light);
		}
		glDisable(GL_STENCIL_TEST);
		
		_geometryBuffer.setDrawBuffer(Final);
		dirLightPass();

		_geometryBuffer.unbind();
		_geometryBuffer.bindForReading();
		_geometryBuffer.setReadBuffer(Final);

		glBlitFramebuffer(0, 0, Display::width, Display::height,
						  0, 0, Display::width, Display::height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		// should restore buffer
		_geometryBuffer.unbind();
	}


	void Technique_Deferred::geometryPass()
	{
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		ObjectRenderer::renderAll(nullptr, _geometryFsPath);

		glDepthMask(GL_FALSE);
	}


	void Technique_Deferred::dirLightPass()
	{
		auto type = Light::LightType_Dir;
		lightPassBase(Light::LightType_Dir);

		for (auto light : Light::getLights(type))
		{
			_lightShaders[type]->setUniform("u_ProjViewWorld", Matrix::identity);
			_lightShaders[type]->setUniform("u_Light.direction", light->transform->getForward());
			_lightShaders[type]->setUniform("u_Light.color", light->color);
			_lightShaders[type]->setUniform("u_Light.ambiendIntensity", light->ambiendIntensity);
			_lightShaders[type]->setUniform("u_Light.diffuseIntensity", light->diffuseIntensity);

			glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);
		}
		glDisable(GL_BLEND);
	}


	void Technique_Deferred::spotLightStencilPass(const Light* light)
	{
		_stencilShader->bind();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		const float& C = light->constant;
		const float& L = light->linear;
		const float& Q = light->quadratic;

		Matrix pv = Camera::currentCamera->getMatrix();
		float lightMax = Utils::max(light->color.v[0], light->color.v[1], light->color.v[2]) * light->diffuseIntensity;
		float depth = (-L + sqrtf(L * L - 4 * Q * (C - (256.0f / MinLightContrib) * C * lightMax))) / (2.0f * Q);

		float cos2 = light->getOuterCutoffCos() * light->getOuterCutoffCos();
		float sin2 = 1.0f - cos2;

		float scaleXY = depth * sqrtf(sin2 / cos2);
		float scaleZ = depth / 2.0f;
		Matrix pvw = pv * light->transform->getMatrix() * Matrix::createScale(Vector3(scaleXY, scaleXY, scaleZ));

		_stencilShader->setUniform("u_ProjViewWorld", pvw);

		auto type = Light::LightType_Spot;
		VertexLayoutBinding::create(_meshes[type], _stencilShader)->bind();
		glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);
	}


	void Technique_Deferred::spotLightPass(const Light* light)
	{
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		auto type = Light::LightType_Spot;
		lightPassBase(type);

		const float& C = light->constant;
		const float& L = light->linear;
		const float& Q = light->quadratic;

		_lightShaders[type]->setUniform("u_Attenuation.constant", C);
		_lightShaders[type]->setUniform("u_Attenuation.linear", L);
		_lightShaders[type]->setUniform("u_Attenuation.quadratic", Q);

		Matrix pv = Camera::currentCamera->getMatrix();
		float lightMax = Utils::max(light->color.v[0], light->color.v[1], light->color.v[2]) * light->diffuseIntensity;
		float depth = (-L + sqrtf(L * L - 4 * Q * (C - (256.0f / MinLightContrib) * C * lightMax))) / (2.0f * Q);

		float cos2 = light->getOuterCutoffCos() * light->getOuterCutoffCos();
		float sin2 = 1.0f - cos2;

		float scaleXY = depth * sqrtf(sin2 / cos2);
		float scaleZ = depth / 2.0f;
		Matrix pvw = pv * light->transform->getMatrix() * Matrix::createScale(Vector3(scaleXY, scaleXY, scaleZ));

		_lightShaders[type]->setUniform("u_ProjViewWorld", pvw);
		_lightShaders[type]->setUniform("u_Light.position", light->transform->getWorldPosition());
		_lightShaders[type]->setUniform("u_Light.direction", light->transform->getForward());
		_lightShaders[type]->setUniform("u_Light.color", light->color);
		_lightShaders[type]->setUniform("u_Light.ambiendIntensity", light->ambiendIntensity);
		_lightShaders[type]->setUniform("u_Light.diffuseIntensity", light->diffuseIntensity);
		_lightShaders[type]->setUniform("u_Light.cutoffCos", light->getCutoffCos());
		_lightShaders[type]->setUniform("u_Light.outerCutoffCos", light->getOuterCutoffCos());

		glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);

		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
	}


	void Technique_Deferred::pointStencilPass(const Light* light)
	{
		_stencilShader->bind();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		const float& C = light->constant;
		const float& L = light->linear;
		const float& Q = light->quadratic;

		Matrix pv = Camera::currentCamera->getMatrix();
		float lightMax = Utils::max(light->color.v[0], light->color.v[1], light->color.v[2]) * light->diffuseIntensity;
		float radius = (-L + sqrtf(L * L - 4 * Q * (C - (256.0f / MinLightContrib) * C * lightMax))) / (2.0f * Q);

		Matrix pvw = pv * light->transform->getMatrix() * Matrix::createScale(Vector3(radius, radius, radius));
		_stencilShader->setUniform("u_ProjViewWorld", pvw);

		auto type = Light::LightType_Point;
		VertexLayoutBinding::create(_meshes[type], _stencilShader)->bind();
		glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);
	}


	void Technique_Deferred::pointLightPass(const Light* light)
	{
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		auto type = Light::LightType_Point;
		lightPassBase(type);

		const float& C = light->constant;
		const float& L = light->linear;
		const float& Q = light->quadratic;

		_lightShaders[type]->setUniform("u_Attenuation.constant", C);
		_lightShaders[type]->setUniform("u_Attenuation.linear", L);
		_lightShaders[type]->setUniform("u_Attenuation.quadratic", Q);

		Matrix pv = Camera::currentCamera->getMatrix();
		float lightMax = Utils::max(light->color.v[0], light->color.v[1], light->color.v[2]) * light->diffuseIntensity;
		float radius = (-L + sqrtf(L * L - 4 * Q * (C - (256.0f / MinLightContrib) * C * lightMax))) / (2.0f * Q);

		// should exclude scaling of light-containing game object?
		Matrix pvw = pv * light->transform->getMatrix() * Matrix::createScale(Vector3(radius, radius, radius));

		_lightShaders[type]->setUniform("u_ProjViewWorld", pvw);
		_lightShaders[type]->setUniform("u_Light.position", light->transform->getWorldPosition());
		_lightShaders[type]->setUniform("u_Light.color", light->color);
		_lightShaders[type]->setUniform("u_Light.ambiendIntensity", light->ambiendIntensity);
		_lightShaders[type]->setUniform("u_Light.diffuseIntensity", light->diffuseIntensity);

		glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);

		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
	}


	void Technique_Deferred::lightPassBase(Light::LightType type)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_lightShaders[type]->bind();

		_lightShaders[type]->setUniform("u_ScreenSize", Vector2(Display::width, Display::height));
		_lightShaders[type]->setUniform("u_CameraPosition", Camera::currentCamera->transform->getWorldPosition());

		const auto& textureNames = _lightShaders[type]->getTextureUnits();
		const auto& colorBuffers = _geometryBuffer.getColorBuffers();
		for (int i = 0; i < textureNames.size(); i++)
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

		VertexLayoutBinding::create(_meshes[type], _lightShaders[type])->bind();
	}
}