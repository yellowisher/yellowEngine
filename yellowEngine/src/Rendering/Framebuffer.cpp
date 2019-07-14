#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/Renderbuffer.hpp"
#include "yellowEngine/Rendering/FrameBuffer.hpp"


namespace yellowEngine
{
	FrameBuffer::FrameBuffer()
	{
		_depthBuffer = nullptr;
		_depthStencilBuffer = nullptr;
		_depthTexture = nullptr;
		_depthCubeMap = nullptr;
	}


	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &_frameBufferHandle);

		for (auto nameTexturePair : _colorBuffers)
		{
			delete(nameTexturePair.second);
		}

		if (_depthBuffer) delete(_depthBuffer);
		if (_depthStencilBuffer) delete(_depthStencilBuffer);
		if (_depthTexture) delete(_depthTexture);
		if (_depthCubeMap) delete(_depthCubeMap);
	}


	void FrameBuffer::addColorAttachment(
		const char* usage, int internalFormat, int width, int height, int format, GLenum type)
	{
		_colorBuffers.push_back({ usage,  new Texture(usage, internalFormat, width, height, format, type) });
	}


	void FrameBuffer::addDepthAttachment(int width, int height)
	{
		_depthBuffer = new RenderBuffer(GL_DEPTH_COMPONENT, width, height);

		_depthMapWidth = width;
		_depthMapHeight = height;
	}


	void FrameBuffer::addDepthTexture(const char* name, int width, int height)
	{
		_depthTexture = new Texture(
			name, GL_DEPTH_COMPONENT, width, height, GL_DEPTH_COMPONENT, GL_FLOAT,
			GL_CLAMP_TO_BORDER, GL_NEAREST);

		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		_depthMapWidth = width;
		_depthMapHeight = height;
	}


	void FrameBuffer::addDepthStencilAttachment(int width, int height)
	{
		_depthStencilBuffer = new RenderBuffer(GL_DEPTH32F_STENCIL8, width, height);

		_depthMapWidth = width;
		_depthMapHeight = height;
	}


	void FrameBuffer::addDepthCubeMap(int internalFormat, int width, int format, int type)
	{
		_depthCubeMap = new CubeMap(internalFormat, width, format, type);
	}


	void FrameBuffer::init()
	{
		Utils::printGLError("Begin frame buffer init");

		glGenFramebuffers(1, &_frameBufferHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferHandle);

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
		if (_depthTexture)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture->_id, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		if (_depthCubeMap)
		{
			//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _depthCubeMap->_cubeMapHandle, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			// should delete framebuffer and textures
			Utils::print("FrameBuffer not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Utils::printGLError("End frame buffer init");
	}


	GLuint FrameBuffer::getDepthMapId()
	{
		if (_depthTexture)
		{
			return _depthTexture->_id;
		}
		if (_depthCubeMap)
		{
			return _depthCubeMap->_cubeMapHandle;
		}
		return 0;
	}


	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferHandle);
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


	void FrameBuffer::bindDepthTexture()
	{
		if (_depthTexture)
		{
			_depthTexture->bind();
		}
		if (_depthCubeMap)
		{
			_depthCubeMap->bind();
		}
	}
}