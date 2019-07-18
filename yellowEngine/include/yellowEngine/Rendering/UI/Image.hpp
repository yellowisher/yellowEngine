#ifndef __H_IMAGE__
#define __H_IMAGE__

#include "yellowEngine/Math/Vector2.hpp"
#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Rendering/Texture.hpp"
#include "yellowEngine/Rendering/UI/UIRenderer.hpp"

namespace yellowEngine
{
	class Image : public UIRenderer
	{
	public:
		Image(Vector2 position, Vector2 size, Texture* texture);
		virtual ~Image();

		void setPosition(Vector2 position);
		Vector2 getPosition() { return _position; };

		void setSize(Vector2 size);
		Vector2 getSize() { return _size; }

		Vector3 color;

	protected:
		virtual void render() override;

	private:
		Texture* _texture;
		Vector2 _position;
		Vector2 _size;
		bool _dirty;

		Vector2 _weight;
		Vector2 _bias;
	};
}

#endif