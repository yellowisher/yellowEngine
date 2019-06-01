#include <gl/glew.h>

#include "ColliderManagerr.hpp"
#include "VertexLayout.hpp"
#include "Transform.hpp"
#include "Collider.hpp"


ShaderProgram* Collider::__wireFrameShader;
ShaderProgram* Collider::__wireFrameWorldShader;
Vector3 Collider::__colliderColor = Vector3(0, 0.8f, 0);
Vector3 Collider::__boundingBoxColor = Vector3(0.8f, 0.8f, 0);

ColliderManager* Collider::manager;


Collider::Collider(GameObject* gameObject) :
	Component(gameObject),
	_transformChangeListener(this),
	_renderPointsChanged(true)
{
	if (__wireFrameShader == nullptr)
	{
		__wireFrameShader = ShaderProgram::create("wireframe.vert", "wireframe.frag");
		__wireFrameWorldShader = ShaderProgram::create("wireframe_world.vert", "wireframe.frag");

		__wireFrameShader->setUniform(__wireFrameShader->getUniform("u_LineColor"), __colliderColor);
		__wireFrameWorldShader->setUniform(__wireFrameWorldShader->getUniform("u_LineColor"), __boundingBoxColor);
	}
	transform->transformChangeNotifier.addListener(&_transformChangeListener);

	_colliderMesh = Mesh::create(VertexLayout({ VertexLayout::Attribute(VertexLayout::Attr_Position, 3) }));
	_colliderBinding = VertexLayoutBinding::create(_colliderMesh, __wireFrameShader);

	_boundingBoxMesh = Mesh::create(VertexLayout({ VertexLayout::Attribute(VertexLayout::Attr_Position, 3) }));
	_boundingBoxBinding = VertexLayoutBinding::create(_boundingBoxMesh, __wireFrameShader);
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
		calcBoundingBox();
		_colliderMesh->updateData(_renderPoints);
		_boundingBoxMesh->updateData(_boundingBoxPoints);
	}
	__wireFrameShader->use();
	__wireFrameShader->updateUniforms(gameObject);
	glBindVertexArray(_colliderBinding->getVertexArrayHandle());
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, _renderPoints.size());

	__wireFrameWorldShader->use();
	__wireFrameWorldShader->updateUniforms(gameObject);
	glBindVertexArray(_boundingBoxBinding->getVertexArrayHandle());
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, _boundingBoxPoints.size());

	glBindVertexArray(NULL);
}


void Collider::calcBoundingBox()
{
	_boundingBoxPoints.clear();
	AABB aabb = getAABB();
	for (int i = 0; i < AABB::Num_Points; i++)
	{
		for (int b = 0; b < 3; b++)
		{
			int bit = 1 << b;
			if (!(i & bit))
			{
				_boundingBoxPoints.push_back(aabb._points[i]);
				_boundingBoxPoints.push_back(aabb._points[i | bit]);
			}
		}
	}
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
