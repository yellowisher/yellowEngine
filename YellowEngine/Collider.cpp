#include <gl/glew.h>

#include "ColliderManagerr.hpp"
#include "VertexLayout.hpp"
#include "Transform.hpp"
#include "Collider.hpp"


ShaderProgram* Collider::__colliderShader;
ColliderManager* Collider::manager;


Collider::Collider(GameObject* gameObject) :
	Component(gameObject),
	_transformChangeListener(this),
	_renderPointsChanged(true)
{
	if (__colliderShader == nullptr)__colliderShader = ShaderProgram::create("collider.vert", "collider.frag");
	transform->transformChangeNotifier.addListener(&_transformChangeListener);

	_mesh = Mesh::create(VertexLayout({ VertexLayout::Attribute(VertexLayout::Attr_Position, 3) }));
	_binding = VertexLayoutBinding::create(_mesh, __colliderShader);
}


Collider::~Collider()
{
}


void Collider::onCreate()
{
	manager->colliderCreated(this);
}


void Collider::onDestroy()
{
	manager->colliderDestroyed(this);
}


Collider::Type Collider::getType()
{
	static Type type = Type_None;
	return type;
}


void Collider::renderCollider()
{
	if (_renderPointsChanged)
	{
		_renderPointsChanged = false;
		calcRenderingData();
		_mesh->updateData(_renderPoints);
	}
	__colliderShader->use();
	__colliderShader->updateUniforms(gameObject);

	glBindVertexArray(_binding->getVertexArrayHandle());
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, _renderPoints.size());
	glBindVertexArray(NULL);
}


void Collider::notify(Event event, void* sender)
{
	switch (event)
	{
		case Event_TransformChanged:
			_renderPointsChanged = true;
			onTransformChange();
			manager->colliderUpdated(this);
			break;
	}
}
