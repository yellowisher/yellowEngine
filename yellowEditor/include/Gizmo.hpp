#ifndef __H_GIZMO__
#define __H_GIZMO__

namespace yellowEditor
{
	void InitGizmo();
	void DrawGizmo();
	void Gizmo_StartDrag(float x, float y);
	void Gizmo_Drag(float x, float y);
	void Gizmo_EndDrag();
}

#endif