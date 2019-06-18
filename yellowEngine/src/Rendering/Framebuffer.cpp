#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/Renderbuffer.hpp"
#include "yellowEngine/Rendering/FrameBuffer.hpp"


namespace yellowEngine
{
	FrameBuffer::FrameBuffer(int textureCount, int width, int height, int format, GLenum type, int internalFormat, int addition)
	{
		glGenFramebuffers(1, &_frameBufferHandle);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferHandle);

		_textures.resize(textureCount);
		GLenum* drawBuffers = new GLenum[textureCount];
		for (int i = 0; i < textureCount; i++)
		{
			_textures[i] = new Texture(width, height, format, type, internalFormat);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i]->_id, 0);
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		// TODO: change as parameterizable
		// add depth or stencil buffer
		Texture* depthTexture = new Texture(width, height, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT32F);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->_id, 0);

		glDrawBuffers(textureCount, drawBuffers);
		delete[](drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			// should delete framebuffer and textures
			Utils::print("FrameBuffer not complete!");
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}


	FrameBuffer::~FrameBuffer()
	{
	}


	void FrameBuffer::bindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferHandle);
	}


	void FrameBuffer::bindForReading()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NULL);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBufferHandle);

		for (size_t i = 0; i < _textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			_textures[i]->bind();
		}
	}
}