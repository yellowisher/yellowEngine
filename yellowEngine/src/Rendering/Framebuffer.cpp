#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/Renderbuffer.hpp"
#include "yellowEngine/Rendering/FrameBuffer.hpp"
#include "..\..\include\yellowEngine\Rendering\FrameBuffer.hpp"


namespace yellowEngine
{
	FrameBuffer::FrameBuffer()
	{
		_depthBuffer = nullptr;
	}


	FrameBuffer::~FrameBuffer()
	{
	}


	void FrameBuffer::addColorAttachment(int internalFormat, int width, int height, int format, GLenum type)
	{
		_colorBuffers.push_back(new Texture(internalFormat, width, height, format, type));
	}


	void FrameBuffer::addDepthAttachment(int width, int height)
	{
		_depthBuffer = new RenderBuffer(GL_DEPTH_COMPONENT, width, height);
	}


	void FrameBuffer::init()
	{
		glGenFramebuffers(1, &_frameBufferHandle);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferHandle);

		GLenum* drawBuffers = new GLenum[_colorBuffers.size()];
		for (int i = 0; i < _colorBuffers.size(); i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorBuffers[i]->_id, 0);
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		if (_depthBuffer)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer->_renderBufferHandle);
		}

		glDrawBuffers((int)_colorBuffers.size(), drawBuffers);
		delete[](drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			// should delete framebuffer and textures
			Utils::print("FrameBuffer not complete!");
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}


	void FrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	}


	void FrameBuffer::bindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferHandle);
	}


	void FrameBuffer::bindForReading()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBufferHandle);

		for (size_t i = 0; i < _colorBuffers.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + (GLenum)i);
			_colorBuffers[i]->bind();
		}
	}


	void FrameBuffer::setBufferToRead(int index)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
	}
}