#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Rendering/Technique/Technique_Deferred.hpp"


namespace yellowEngine
{
	Technique_Deferred::Technique_Deferred()
	{
		// position
		_geometryBuffer.addColorAttachment(GL_RGB32F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		
		// normal
		_geometryBuffer.addColorAttachment(GL_RGB32F, Display::width, Display::height, GL_RGB, GL_FLOAT);
		
		// diffuse & spec
		_geometryBuffer.addColorAttachment(GL_RGBA32F, Display::width, Display::height, GL_RGBA, GL_UNSIGNED_BYTE);
		
		_geometryBuffer.addDepthAttachment(Display::width, Display::height);
		_geometryBuffer.init();

		_geometryFsPath = "Shader/deferred_geometry.frag";
	}


	Technique_Deferred::~Technique_Deferred()
	{
	}


	void Technique_Deferred::_renderScene()
	{
		// geometry pass
		glEnable(GL_DEPTH_TEST); 
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		_geometryBuffer.bindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ObjectRenderer::renderAll(Camera::getMainCamera(), nullptr, _geometryFsPath);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);

		// lighting pass
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_geometryBuffer.unbind();
		_geometryBuffer.bindForReading();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_geometryBuffer.setBufferToRead(0);
		glBlitFramebuffer(
			0, 0, Display::width, Display::height,
			0, 0, Display::width / 2, Display::height / 2,
			GL_COLOR_BUFFER_BIT, GL_LINEAR
		);

		_geometryBuffer.setBufferToRead(1);
		glBlitFramebuffer(
			0, 0, Display::width, Display::height,
			Display::width / 2, 0, Display::width, Display::height / 2,
			GL_COLOR_BUFFER_BIT, GL_LINEAR
		);

		_geometryBuffer.setBufferToRead(2);
		glBlitFramebuffer(
			0, 0, Display::width, Display::height,
			0, Display::height / 2, Display::width / 2, Display::height,
			GL_COLOR_BUFFER_BIT, GL_LINEAR
		);
	}
}