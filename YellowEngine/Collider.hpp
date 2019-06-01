#ifndef __H_COLLIDER__
#define __H_COLLIDER__

#include <vector>

#include "Component.hpp"
#include "Vector3.hpp"
#include "Event.hpp"
#include "Mesh.hpp"
#include "AABB.hpp"
#include "ShaderProgram.hpp"
#include "VertexLayoutBinding.hpp"

class ColliderManager;

class Collider : public Component, public INotifiable
{
public:
	enum Type
	{
		Type_None = -1,
		Type_Box,
		Type_Sphere
	};

	static ColliderManager* manager;

	Collider(GameObject* gameObject);
	virtual ~Collider();

	virtual bool isCollideWith(Collider* other) = 0;
	virtual Type getType();
	virtual const AABB getAABB() = 0;

	virtual void onCreate() override;
	virtual void onDestroy() override;

protected:
	EventListener _transformChangeListener;

	virtual void onTransformChange() = 0;

private:
	void notify(Event event, void* sender) override;

// For collider & bounding box rendering; should be removed?
public:
	void renderCollider();

protected:
	virtual void calcRenderingData() = 0;
	void calcBoundingBox();

	std::vector<Vector3> _renderPoints;
	std::vector<Vector3> _boundingBoxPoints;
	bool _renderPointsChanged;

private:
	static ShaderProgram* __wireFrameShader;
	static ShaderProgram* __wireFrameWorldShader;
	static Vector3 __colliderColor;
	static Vector3 __boundingBoxColor;

	Mesh* _colliderMesh;
	Mesh* _boundingBoxMesh;
	VertexLayoutBinding* _colliderBinding;
	VertexLayoutBinding* _boundingBoxBinding;
};

#endif