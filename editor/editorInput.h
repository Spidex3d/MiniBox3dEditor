#pragma once
#include <guiWindow.h>
#include <camera.h>
#include <editorState.h>


class inputHandler {
public:
	inputHandler() = default;
	~inputHandler() = default;

	void Timer();
	

	void processInput(guiWin& app, guiWin::gui_window* window, Camera& camera);
	
	void editMode(guiWin& app, guiWin::gui_window* window, EditorState& editor);
	
	
	bool cubeSelected = false;
private:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

};