#include "ColliderManagerr.hpp"
#include "VertexLayout.hpp"
#include "Transform.hpp"
#include "Collider.hpp"


ShaderProgram* Collider::__colliderShader;
ShaderProgram* Collider::__boundingBoxShader;


Collider::Collider(GameObject* gameObject) :
	Component(gameObject),
	_renderPointsChanged(true)
{
	if (__colliderShader == nullptr)
	{
		__colliderShader = ShaderProgram::create("wireframe_collider.vert", "wireframe_collider.frag");
		__boundingBoxShader = ShaderProgram::create("wireframe_boundingbox.vert", "wireframe_boundingbox.frag");
	}

	VertexLayout layout = VertexLayout({ VertexLayout::Attribute(VertexLayout::Attr_Position, 3) });
	_colRenderer = Renderer(layout, __colliderShader, gameObject);
	_bbRenderer = Renderer(layout, __boundingBoxShader, gameObject);
}


Collider::~Collider()
{
}


void Collider::onCreate()
{
	_transformChangeListener.setParent(this);
	transform->transformChangeNotifier.addListener(&_transformChangeListener);

	ColliderManager::getInstance()->colliderCreated(this);
}


void Collider::onDestroy()
{
	ColliderManager::getInstance()->colliderDestroyed(this);
}


void Collider::renderCollider()
{
	if (_renderPointsChanged)
	{
		_renderPointsChanged = false;
		setColliderRenderingData();
		_bbRenderer.setData(getBoundingBox());
	}

	_colRenderer.render();
	_bbRenderer.render();
}


void Collider::notify(Event event, void* sender)
{
	switch (event)
	{
		case Event_TransformChanged:
			_renderPointsChanged = true;
			onTransformChange();
			ColliderManager::getInstance()->colliderUpdated(this);
			break;
	}
}
