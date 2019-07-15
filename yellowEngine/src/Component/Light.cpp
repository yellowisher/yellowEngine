#include <glad/glad.h>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Component/Camera.hpp"
#include "yellowEngine/Component/Light.hpp"
#include "yellowEngine/Component/Transform.hpp"


namespace yellowEngine
{
	static constexpr float MinLightContrib = 5.0f;

	COMPONENT_IMPL(Light)
	std::vector<Light*> Light::__lights[Num_LightType];

	const std::vector<Light*>& Light::getLights(LightType type)
	{
		return __lights[type];
	}


	Light::Light(GameObject* gameObject) :Component(gameObject)
	{
		color = Vector3(1.0f, 1.0f, 1.0f);
		ambientIntensity = 0.1f;
		diffuseIntensity = 1.0f;

		setCutoff(12.5f, 17.5f);

		constant = 1.0f;
		linear = 0.14f;
		quadratic = 0.07f;

		_type = LightType_Dir;
		__lights[LightType_Dir].push_back(this);
		_projectionMatrix = Matrix::createOrthographic(50.0f, 50.0f, -1.0f, 50.0f);

		castShadow = true;
		// really should light holds frame buffer?
		_shadowBuffer = new FrameBuffer();
		_shadowBuffer->addDepthTexture("u_ShadowDepthMap", Display::width, Display::height);
		_shadowBuffer->init();

		valueChanged = false;
	}


	Light::~Light()
	{
		for (int i = 0; i < __lights[_type].size(); i++)
		{
			if (__lights[_type][i] == this)
			{
				__lights[_type][i] = __lights[_type].back();
				__lights[_type].pop_back();
				break;
			}
		}
	}


	Light* Light::setType(LightType type)
	{
		bool found = false;
		for (int lt = 0; lt < Num_LightType; lt++)
		{
			for (int i = 0; i < __lights[lt].size(); i++)
			{
				if (__lights[lt][i] == this)
				{
					__lights[lt][i] = __lights[lt].back();
					__lights[lt].pop_back();
					found = true;
					break;
				}
			}
			if (found) break;
		}

		_type = type;
		__lights[_type].push_back(this);

		valueChanged = true;
		
		return this;
	}


	void Light::setCutoff(float inner, float outer)
	{
		_cutoffDegree = inner;
		_outerCutoffDegree = outer;

		_cutoffCos = cosf(Utils::deg2rad(_cutoffDegree));
		_outerCutoffCos = cosf(Utils::deg2rad(_outerCutoffDegree));
	}


	float Light::getCutoffCos() const
	{
		return _cutoffCos;
	}


	float Light::getOuterCutoffCos() const
	{
		return _outerCutoffCos;
	}


	Vector3 Light::getDirection() const
	{
		return transform->getForward();
	}


	float Light::getRange()
	{
		float lightMax = Utils::max(color.v[0], color.v[1], color.v[2]) * diffuseIntensity;

		return 
			(-linear + sqrtf(linear * linear - 4 * quadratic * 
			(constant - (256.0f / MinLightContrib) * constant * lightMax))) 
			/ (2.0f * quadratic);
	}


	Matrix Light::getProjMatrix()
	{
		return _projectionMatrix;
	}


	Matrix Light::getProjViewMatrix()
	{
		return _projectionMatrix * transform->getInverseMatrix();
	}


	FrameBuffer* Light::getShadowBuffer()
	{
		if (valueChanged)
		{
			valueChanged = false;
			if (_shadowBuffer)
			{
				delete(_shadowBuffer);
				_shadowBuffer = nullptr;
			}

			if (castShadow)
			{
				_shadowBuffer = new FrameBuffer();
				if (_type == LightType_Dir)
				{
					_projectionMatrix = Matrix::createOrthographic(50.0f, 50.0f, -1.0f, 50.0f);
					_shadowBuffer->addDepthTexture("u_ShadowDepthMap", Display::width, Display::height);
				}
				else
				{
					// TODO:: calculate projection parameters based on attenuation range
					_zNear = 1.0f;
					_zFar = 100.0f;

					if (_type == LightType_Spot)
					{
						_projectionMatrix = Matrix::createPerspective(90.0f, Display::aspectRatio, _zNear, _zFar);
						_shadowBuffer->addDepthTexture("u_ShadowDepthMap", Display::width, Display::height);
					}
					else
					{
						_projectionMatrix = Matrix::createPerspective(90.0f, 1.0f, _zNear, _zFar);
						_shadowBuffer->addDepthAttachment(Display::width, Display::width);
						_shadowBuffer->addDepthCubeMap(GL_R32F, Display::width, GL_RED, GL_FLOAT);
					}
				}

				_shadowBuffer->init();
			}
		}

		return _shadowBuffer;
	}


	void Light::updateUniforms(Shader* shader)
	{
		shader->setUniform("u_Light.color", color);
		shader->setUniform("u_Light.castShadow", castShadow);
		shader->setUniform("u_Light.ambientIntensity", ambientIntensity);
		shader->setUniform("u_Light.diffuseIntensity", diffuseIntensity);
		shader->setUniform("u_CameraPosition", Camera::currentCamera->transform->getWorldPosition());

		shader->setUniform("u_ScreenSize", Vector2(Display::width, Display::height));

		if (_type == LightType_Dir)
		{
			shader->setUniform("u_ProjViewWorld", Matrix::identity);
			shader->setUniform("u_LightProjView", getProjViewMatrix());
			shader->setUniform("u_Light.direction", transform->getForward());
		}
		else
		{
			shader->setUniform("u_Attenuation.constant", constant);
			shader->setUniform("u_Attenuation.linear", linear);
			shader->setUniform("u_Attenuation.quadratic", quadratic);
			shader->setUniform("u_Light.position", transform->getWorldPosition());

			Matrix pv = Camera::currentCamera->getMatrix();
			float range = getRange();

			if (_type == LightType_Spot)
			{
				float cos2 = _outerCutoffCos * _outerCutoffCos;
				float sin2 = 1.0f - cos2;

				float scaleXY = range * sqrtf(sin2 / cos2);
				float scaleZ = range / 2.0f;
				Matrix pvw = pv * transform->getMatrix() * Matrix::createScale(Vector3(scaleXY, scaleXY, scaleZ));

				shader->setUniform("u_ProjViewWorld", pvw);
				shader->setUniform("u_LightProjView", getProjViewMatrix());
				shader->setUniform("u_Light.direction", transform->getForward());
				shader->setUniform("u_Light.cutoffCos", getCutoffCos());
				shader->setUniform("u_Light.outerCutoffCos", getOuterCutoffCos());
			}
			else
			{
				// should exclude scaling of light-containing game object?
				Matrix pvw = pv * transform->getMatrix() * Matrix::createScale(Vector3(range, range, range));
				shader->setUniform("u_ProjViewWorld", pvw);
			}
		}
	}


	void Light::onValueChanged()
	{
		setType(_type);
		if (_type == LightType_Spot) setCutoff(_cutoffDegree, _outerCutoffDegree);
	}
}