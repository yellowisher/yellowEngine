#include <vector>

#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Rendering/UIRenderer.hpp"


namespace yellowEngine
{
	struct Image
	{
		Texture* texture;
		float weightX;
		float biasX;
		float weightY;
		float biasY;
	};

	static std::vector<Image> images;

	void UIRenderer::drawImage(Texture* image, Vector2 min, Vector2 max)
	{
		float ndcMinX = 2.0f * min.x / Display::width - 1.0f;
		float ndcMaxX = 2.0f * max.x / Display::width - 1.0f;

		float weightX = (ndcMaxX - ndcMinX) * 0.5f;
		float biasX = (ndcMinX + ndcMaxX) * 0.5f;

		float ndcMinY = 2.0f * min.y / Display::height - 1.0f;
		float ndcMaxY = 2.0f * max.y / Display::height - 1.0f;

		float weightY = (ndcMaxY - ndcMinY) * 0.5f;
		float biasY = (ndcMinY + ndcMaxY) * 0.5f;

		images.push_back({ image, weightX, biasX, weightY, biasY });
	}


	void UIRenderer::render()
	{
		static Mesh* quad = Mesh::create("./res/Mesh/quad.obj");
		static Shader* shader = Shader::create("./res/Shader/ui.vert", "./res/Shader/ui.frag");
		static VertexLayoutBinding* binding = VertexLayoutBinding::create(quad, shader);

		shader->bind();
		binding->bind();

		for (auto image : images)
		{
			shader->setUniform("u_WeightX", image.weightX);
			shader->setUniform("u_BiasX", image.biasX);
			shader->setUniform("u_WeightY", image.weightY);
			shader->setUniform("u_BiasY", image.biasY);

			glActiveTexture(GL_TEXTURE0);
			image.texture->bind();
			glDrawElements(GL_TRIANGLES, quad->getVertexCount(), GL_UNSIGNED_INT, 0);
		}

		images.clear();
	}
}