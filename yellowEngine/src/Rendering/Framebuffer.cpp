#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/Renderbuffer.hpp"
#include "yellowEngine/Rendering/FrameBuffer.hpp"


namespace yellowEngine
{
	FrameBuffer::FrameBuffer()
	{
		_depthBuffer = nullptr;
		_depthStencilBuffer = nullptr;
	}


	FrameBuffer::~FrameBuffer()
	{
	}


	void FrameBuffer::addColorAttachment(
		const char* usage, int internalFormat, int width, int height, int format, GLenum type)
	{
		_colorBuffers.push_back({ usage,  new Texture(internalFormat, width, height, format, type) });
	}


	void FrameBuffer::addDepthAttachment(int width, int height)
	{
		_depthBuffer = new RenderBuffer(GL_DEPTH_COMPONENT, width, height);
	}


	void FrameBuffer::addDepthStencilAttachment(int width, int height)
	{
		_depthStencilBuffer = new RenderBuffer(GL_DEPTH32F_STENCIL8, width, height);
	}


	void FrameBuffer::init()
	{
		glGenFramebuffers(1, &_frameBufferHandle);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferHandle);

		for (int i = 0; i < _colorBuffers.size(); i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorBuffers[i].second->_id, 0);
		}

		if (_depthBuffer)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer->_renderBufferHandle);
		}
		if (_depthStencilBuffer)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilBuffer->_renderBufferHandle);
		}

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


	void FrameBuffer::bindForDrawing()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferHandle);
	}


	void FrameBuffer::bindForReading()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBufferHandle);
	}


	void FrameBuffer::setDrawBuffer(int index, int count)
	{
		if (index == -1)
		{
			glDrawBuffer(GL_NONE);
		}
		else if (count == 1)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + index);
		}
		else
		{
			GLenum* drawBuffers = new GLenum[count];
			for (int i = 0; i < count; i++)
			{
				drawBuffers[i] = GL_COLOR_ATTACHMENT0 + index + i;
			}
			glDrawBuffers(count, drawBuffers);
			delete[](drawBuffers);
		}
	}


	void FrameBuffer::setReadBuffer(int index)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
	}


	const std::vector<std::pair<std::string, Texture*>>& FrameBuffer::getColorBuffers()
	{
		return _colorBuffers;
	}
}