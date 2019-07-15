#include "yellowEngine/yellowEngine.hpp"
#include "Editor.hpp"
#include "Gizmo.hpp"

using namespace yellowEngine;

namespace yellowEditor
{
	static const int MIN_GIZMO_CLICK_MAGNITUDE = 100;

	static Shader* gizmoShader;
	static Renderer* gizmoRenderer;

	static Vector2 screenBegin;
	static Vector2 screenEnds[3];
	static Vector2 lastCursorPos;
	static int dragging = -1;
	static float modelLength;

	// for calculate dragging distance
	static Vector2 BtoE;
	static float BtoEMagnitude;

	static Vector4 ndcBegin;
	static Vector4 ndcEnds[3];

	static const Vector3 selectColor = Vector3(1.0f, 1.0f, 1.0f);
	static const Vector3 origColors[3] = {
		Vector3(1.0f, 0, 0),
		Vector3(0, 1.0f, 0),
		Vector3(0, 0, 1.0f)
	};

	static Vector3 colors[3] = {
		origColors[0],
		origColors[1],
		origColors[2]
	};

	static Vector3 dirs[3] = {
		Vector3(1.0f, 0, 0),
		Vector3(0, 1.0f, 0),
		Vector3(0, 0, -1.0f)
	};


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
			auto pvm = Editor::getEditorCamera()->getMatrix() * transform->getMatrix();

			Vector4 ndcOrigin = pvm * Vector4(0, 0, 0, 1.0f);
			ndcBegin = ndcOrigin;
			ndcOrigin = ndcOrigin / ndcOrigin.w;

			screenBegin = Vector2(
				Editor::getGameWindow().width * 0.5f * (ndcOrigin.x + 1.0f),
				Editor::getGameWindow().height * 0.5f * (-ndcOrigin.y + 1.0f));

			for (int i = 0; i < 3; i++)
			{
				Vector4 ndcTarget = pvm * Vector4(dirs[i], 1.0f);
				ndcTarget = ndcTarget / ndcTarget.w;

				Vector4 dir = ndcTarget - ndcOrigin;
				dir.normalize();
				ndcTarget = ndcOrigin + dir;// *0.4f;
				ndcEnds[i] = ndcTarget;

				screenEnds[i] = Vector2(
					Editor::getGameWindow().width * 0.5f * (ndcTarget.x + 1.0f),
					Editor::getGameWindow().height * 0.5f * (-ndcTarget.y + 1.0f));

				gizmoShader->setUniform("u_LineColor", colors[i]);
				gizmoRenderer->data = { ndcOrigin, ndcTarget };
				gizmoRenderer->bufferData();
				gizmoRenderer->render();
			}
		}
	}


	void Gizmo_StartDrag(float x, float y)
	{
		Vector2 point = Vector2(x, y);

		int index = -1;
		float magnitude = MIN_GIZMO_CLICK_MAGNITUDE;

		for (int i = 0; i < 3; i++)
		{
			float lineMag = (screenEnds[i] - screenBegin).magnitude();
			float t = Utils::max(0.0f, Utils::min(1.0f, ((point - screenBegin)*(screenEnds[i] - screenBegin)) / lineMag));
			Vector2 projection = screenBegin + (screenEnds[i] - screenBegin) * t;

			float mag = (point - projection).magnitude();
			if (mag < magnitude)
			{
				magnitude = mag;
				index = i;
			}
		}

		if (index != -1)
		{
			dragging = index;
			colors[index] = selectColor;
			lastCursorPos = Vector2(x, y);

			BtoE = screenEnds[index] - screenBegin;
			BtoEMagnitude = BtoE.magnitude();

			auto inv = Editor::getEditorCamera()->getMatrix() * Editor::getSelectedTransform()->getMatrix();
			inv = ~inv;
			auto modelBegin = inv * ndcBegin;
			auto modelEnd = inv * ndcEnds[index];

			modelLength = sqrtf((modelEnd - modelBegin).magnitude());
		}
	}


	void Gizmo_Drag(float x, float y)
	{
		if (dragging != -1)
		{
			Vector2 p0 = lastCursorPos;
			Vector2 p1 = Vector2(x, y);




			float lineMag = BtoEMagnitude;
			auto inv = Editor::getEditorCamera()->getMatrix() * Editor::getSelectedTransform()->getMatrix();
			inv = ~inv;

			float t = Utils::max(0.0f, Utils::min(1.0f, ((p0 - screenBegin)*(screenEnds[dragging] - screenBegin)) / lineMag));
			Vector3 projection = ndcBegin + (ndcEnds[dragging] - ndcBegin) * t;
			Vector4 world0 = inv * Vector4(projection, 1.0f);

			t = Utils::max(0.0f, Utils::min(1.0f, ((p1 - screenBegin)*(screenEnds[dragging] - screenBegin)) / lineMag));
			projection = ndcBegin + (ndcEnds[dragging] - ndcBegin) * t;
			Vector4 world1 = inv * Vector4(projection, 1.0f);

			Editor::getSelectedTransform()->translate(world1 - world0);


			
			//float ratio = (BtoE * (p1 - p0)) / BtoEMagnitude;

			//ratio *= (dragging == 2 ? -1.0f : 1.0f);
			////amount = amount / Editor::getGameWindow().width * 0.5f;
			//ratio *= modelLength;
			//Vector3 move;
			//move.v[dragging] = ratio;
			//Editor::getSelectedTransform()->translate(move);

			lastCursorPos = Vector2(x, y);
		}
	}


	void Gizmo_EndDrag()
	{
		if (dragging != -1)
		{
			colors[dragging] = origColors[dragging];
			dragging = -1;
		}
	}
}