#pragma once
#include <guiWindow.h>
#include <camera.h>
#include <editorState.h>
#include <boXGL.h>
#include <mesh\boxMesh.h>


class inputHandler {
public:
	inputHandler() = default;
	~inputHandler() = default;

	void Timer();
	

	void processInput(guiWin& app, guiWin::gui_window* window, Camera& camera);
	
	void editMode(guiWin& app, guiWin::gui_window* window, EditorState& editor);

	void editCubeSelected(guiWin& app, guiWin::gui_window* window, boXGL& gl, const Camera& camera,
		EditorState& editor, const vec3& cubePosition, bool& cubeSelected);
	
    int PickVertex(guiWin& app, guiWin::gui_window* window, boXGL& gl, const Camera& camera, const boXMesh& mesh, float pickRadius);

    int PickEdge(guiWin& app, guiWin::gui_window* window, boXGL& gl, const Camera& camera, const boXMesh& mesh, float pickRadius);

    void editSelection(guiWin& app, guiWin::gui_window* window, boXGL& gl, const Camera& camera, EditorState& editor, const boXMesh& mesh);
	
	bool cubeSelected = false;
private:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

};