#include "yellowEngine/System/Game.hpp"
#include "yellowEngine/Rendering/Technique/Technique_Deferred.hpp"


namespace yellowEngine
{
	Technique_Deferred::Technique_Deferred()
	{
		// position
		_geometryBuffer.addColorAttachment(GL_RGB32F, Game::getWidth(), Game::getHeight(), GL_RGB, GL_FLOAT);
		
		// normal
		_geometryBuffer.addColorAttachment(GL_RGB32F, Game::getWidth(), Game::getHeight(), GL_RGB, GL_FLOAT);

		// diffuse & spec
		_geometryBuffer.addColorAttachment(GL_RGBA32F, Game::getWidth(), Game::getHeight(), GL_RGBA, GL_UNSIGNED_BYTE);

		_geometryBuffer.addDepthAttachment(Game::getWidth(), Game::getHeight());
		
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
			0, 0, Game::getWidth(), Game::getHeight(),
			0, 0, Game::getWidth() / 2, Game::getHeight() / 2,
			GL_COLOR_BUFFER_BIT, GL_LINEAR
		);

		_geometryBuffer.setBufferToRead(1);
		glBlitFramebuffer(
			0, 0, Game::getWidth(), Game::getHeight(),
			Game::getWidth() / 2, 0, Game::getWidth(), Game::getHeight() / 2,
			GL_COLOR_BUFFER_BIT, GL_LINEAR
		);

		_geometryBuffer.setBufferToRead(2);
		glBlitFramebuffer(
			0, 0, Game::getWidth(), Game::getHeight(),
			0, Game::getHeight() / 2, Game::getWidth() / 2, Game::getHeight(),
			GL_COLOR_BUFFER_BIT, GL_LINEAR
		);
	}
}