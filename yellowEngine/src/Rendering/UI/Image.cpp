#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Rendering/Shader.hpp"
#include "yellowEngine/Rendering/VertexLayoutBinding.hpp"
#include "yellowEngine/Rendering/UI/Image.hpp"


namespace yellowEngine
{
	Image::Image(Texture* texture, Vector2 origin):
		_texture(texture)
	{
		color = Vector3(1.0f, 1.0f, 1.0f);
		_dirty = false;
		setBound(origin);
	}


	Image::Image(Vector2 position, Vector2 size, Texture* texture) :
		_position(position),
		_size(size),
		_texture(texture)
	{
		color = Vector3(1.0f, 1.0f, 1.0f);
		_dirty = true;
	}


	Image::~Image()
	{
	}


	void Image::setPosition(Vector2 position)
	{
		_dirty = true;
		_position = position;
	}


	void Image::setSize(Vector2 size)
	{
		_dirty = true;
		_size = size;
	}


	void Image::setBound(Vector2 origin)
	{
		setBound(origin, origin + Vector2(_texture->width, _texture->height));
	}


	void Image::setBound(Vector2 min, Vector2 max)
	{
		_dirty = false;
		calculateWeightBias(min, max);
	}


	void Image::calculateWeightBias(Vector2 screenMin, Vector2 screenMax)
	{
		Vector2 ndcMin = Vector2(
			2.0f * screenMin.x / Display::width - 1.0f,
			2.0f * screenMin.y / Display::height - 1.0f);

		Vector2 ndcMax = Vector2(
			2.0f * screenMax.x / Display::width - 1.0f,
			2.0f * screenMax.y / Display::height - 1.0f);

		Vector2 ndcDif = ndcMax - ndcMin;

		_weight = (ndcMax - ndcMin) * 0.5f;
		_bias = (ndcMax + ndcMin) * 0.5f;
	}


	void Image::render()
	{
		static Mesh* quad = Mesh::create("./res/Mesh/quad.obj");
		static Shader* shader = Shader::create("./res/Shader/image.vert", "./res/Shader/image.frag");
		static VertexLayoutBinding* binding = VertexLayoutBinding::create(quad, shader);

		if (_dirty)
		{
			_dirty = false;
			calculateWeightBias(_position - _size / 2.0f, _position + _size / 2.0f);
		}

		shader->bind();
		binding->bind();

		shader->setUniform("u_Weight", _weight);
		shader->setUniform("u_Bias", _bias);
		shader->setUniform("u_Color", color);

		glActiveTexture(GL_TEXTURE0);
		_texture->bind();
		glDrawElements(GL_TRIANGLES, quad->getVertexCount(), GL_UNSIGNED_INT, 0);
	}
}