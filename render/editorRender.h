#pragma once

#include <guiWindow.h>
#include <boXGL.h>
#include <camera.h>
#include <editorState.h>
#include <mesh\boxMesh.h>

class editorRender
{
public:
    void DrawEditorOverlays(
        guiWin::gui_window* window,
        boXGL& gl,
        const Camera& camera,
        const EditorState& editor,
        const boXMesh& cube,
        const vec3& cubePosition,
        bool cubeSelected);

	void selectVertex(guiWin::gui_window* window, boXGL& gl, const Camera& camera, boXMesh& mesh, const vec3& mousePos, float pickRadius);
	void selectEdge(guiWin::gui_window* window, boXGL& gl, const Camera& camera, boXMesh& mesh, const vec3& mousePos, float pickRadius);
	void selectFace(guiWin::gui_window* window, boXGL& gl, const Camera& camera, boXMesh& mesh, const vec3& mousePos, float pickRadius);
};
