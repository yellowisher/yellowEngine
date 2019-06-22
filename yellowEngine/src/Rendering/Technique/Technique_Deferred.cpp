#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Component/Light.hpp"
#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/Technique/Technique_Deferred.hpp"


namespace yellowEngine
{
	Technique_Deferred::Technique_Deferred()
	{
		_geometryBuffer.addColorAttachment("u_PositionMap", GL_RGB32F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_geometryBuffer.addColorAttachment("u_NormalMap", GL_RGB32F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		_geometryBuffer.addColorAttachment("u_ColorMap", GL_RGBA32F, Display::width, Display::height, GL_RGBA, GL_UNSIGNED_BYTE);
		_geometryBuffer.addDepthAttachment(Display::width, Display::height);
		_geometryBuffer.init();

		_geometryFsPath = "Shader/deferred_geometry.frag";

		_meshes[Light::LightType_Dir  ] = Mesh::create("Mesh/quad.obj");
		//_meshes[Light::LightType_Spot ] = Mesh::create("Mesh/cone.obj");
		_meshes[Light::LightType_Point] = Mesh::create("Mesh/sphere.obj");

		_lightShaders[Light::LightType_Dir  ] = Shader::create("Shader/deferred_light.vert", "Shader/deferred_light_dir.frag");
		_lightShaders[Light::LightType_Spot ] = Shader::create("Shader/deferred_light.vert", "Shader/deferred_light_spot.frag");
		_lightShaders[Light::LightType_Point] = Shader::create("Shader/deferred_light.vert", "Shader/deferred_light_point.frag");
	}


	Technique_Deferred::~Technique_Deferred()
	{
	}


	void Technique_Deferred::_renderScene()
	{
		geometryPass();

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_geometryBuffer.unbind();
		_geometryBuffer.bindForReading();
		glClear(GL_COLOR_BUFFER_BIT);

		dirLightPass();
		spotLightPass();
		pointLightPass();
	}


	void Technique_Deferred::geometryPass()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		_geometryBuffer.bindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ObjectRenderer::renderAll(Camera::getMainCamera(), nullptr, _geometryFsPath);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
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
	}


	void Technique_Deferred::spotLightPass()
	{
		auto type = Light::LightType_Spot;
		lightPassBase(Light::LightType_Spot);

		const float& C = Light::constant;
		const float& L = Light::linear;
		const float& Q = Light::quadratic;

		_lightShaders[type]->setUniform("u_Attenuation.constant", C);
		_lightShaders[type]->setUniform("u_Attenuation.linear", L);
		_lightShaders[type]->setUniform("u_Attenuation.quadratic", Q);

		Matrix pv = Camera::getMainCamera()->getMatrix();
		for (auto light : Light::getLights(type))
		{
			float lightMax = Utils::max(light->color.v[0], light->color.v[1], light->color.v[2]) * light->diffuseIntensity;
			float radius = (-L + sqrtf(L * L - 4 * Q * (C - 256.0f * C * lightMax))) / (2.0f * Q);

			Matrix pvw = pv * light->transform->getMatrix() * Matrix::createScale(Vector3(radius, radius, radius));

			_lightShaders[type]->setUniform("u_ProjViewWorld", pvw);
			_lightShaders[type]->setUniform("u_Light.position", light->transform->getWorldPosition());
			_lightShaders[type]->setUniform("u_Light.direction", light->transform->getForward());
			_lightShaders[type]->setUniform("u_Light.color", light->color);
			_lightShaders[type]->setUniform("u_Light.ambiendIntensity", light->ambiendIntensity);
			_lightShaders[type]->setUniform("u_Light.diffuseIntensity", light->diffuseIntensity);
			_lightShaders[type]->setUniform("u_Light.cutoffCos", light->cutoffCos);
			_lightShaders[type]->setUniform("u_Light.outerCutoffCos", light->outerCutoffCos);

			glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);
		}
	}


	void Technique_Deferred::pointLightPass()
	{
		auto type = Light::LightType_Point;
		lightPassBase(type);

		const float& C = Light::constant;
		const float& L = Light::linear;
		const float& Q = Light::quadratic;

		_lightShaders[type]->setUniform("u_Attenuation.constant", C);
		_lightShaders[type]->setUniform("u_Attenuation.linear", L);
		_lightShaders[type]->setUniform("u_Attenuation.quadratic", Q);

		Matrix pv = Camera::getMainCamera()->getMatrix();
		for (auto light : Light::getLights(type))
		{
			float lightMax = Utils::max(light->color.v[0], light->color.v[1], light->color.v[2]) * light->diffuseIntensity;
			float radius = (-L + sqrtf(L * L - 4 * Q * (C - 256.0f * C * lightMax))) / (2.0f * Q);

			// should exclude scaling of light-containing game object?
			Matrix pvw = pv * light->transform->getMatrix() * Matrix::createScale(Vector3(radius, radius, radius));

			_lightShaders[type]->setUniform("u_ProjViewWorld", pvw);
			_lightShaders[type]->setUniform("u_Light.position", light->transform->getWorldPosition());
			_lightShaders[type]->setUniform("u_Light.color", light->color);
			_lightShaders[type]->setUniform("u_Light.ambiendIntensity", light->ambiendIntensity);
			_lightShaders[type]->setUniform("u_Light.diffuseIntensity", light->diffuseIntensity);

			glDrawElements(GL_TRIANGLES, _meshes[type]->getVertexCount(), GL_UNSIGNED_INT, 0);
		}
	}


	void Technique_Deferred::lightPassBase(Light::LightType type)
	{
		_lightShaders[type]->bind();

		_lightShaders[type]->setUniform("u_ScreenSize", Vector2(Display::width, Display::height));
		_lightShaders[type]->setUniform("u_CameraPosition", Camera::getMainCamera()->transform->getWorldPosition());

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