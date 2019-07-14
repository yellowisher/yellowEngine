#include "yellowEngine/yellowEngine.hpp"
#include "Editor.hpp"
#include "Gizmo.hpp"

using namespace yellowEngine;

namespace yellowEditor
{
	static Shader* gizmoShader;
	static Renderer* gizmoRenderer;


	static void DrawGizmoLine(Transform* transform, Matrix& pv, Vector4 screenOrigin, Vector3 lineDir, Vector3 color)
	{
		Vector3 target = transform->getWorldPosition() + lineDir;
		Vector4 screenTarget = pv * Vector4(target, 1.0f);
		screenTarget = screenTarget / screenTarget.w;

		Vector4 dir = screenTarget - screenOrigin;
		dir.normalize();
		screenTarget = screenOrigin + dir * 0.4f;

		gizmoShader->setUniform("u_LineColor", color);
		gizmoRenderer->data = { screenOrigin, screenTarget };
		gizmoRenderer->bufferData();
		gizmoRenderer->render();
	}


	void InitGizmo()
	{
		VertexLayout layout({ Attr_Position });
		gizmoShader = Shader::create("./res/Shader/gizmo.vert", "./res/Shader/gizmo.frag");
		gizmoRenderer = new Renderer(layout, gizmoShader);
	}


	void DrawGizmo()
	{
		Transform* transform = Editor::getSelectedTransform();
		if (transform != nullptr)
		{
			gizmoShader->bind();
			auto pv = Editor::getEditorCamera()->getMatrix();

			Vector3 origin = transform->getWorldPosition();
			Vector4 screenOrigin = pv * Vector4(origin, 1.0f);
			screenOrigin = screenOrigin / screenOrigin.w;

			DrawGizmoLine(transform, pv, screenOrigin, transform->getRight(), Vector3(1.0f, 0, 0));
			DrawGizmoLine(transform, pv, screenOrigin, transform->getUp(), Vector3(0, 1.0f, 0));
			DrawGizmoLine(transform, pv, screenOrigin, transform->getForward(), Vector3(0, 0, 1.0f));
		}
	}
}