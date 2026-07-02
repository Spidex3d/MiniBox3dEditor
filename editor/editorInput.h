#pragma once
#include <guiWindow.h>
#include <camera.h>

//extern Camera camera;
//extern float lastX;
//extern float lastY;
//extern bool firstMouse;
//extern bool mouse;
//extern float deltaTime;
//extern float lastFrame;
//extern float fov;



class inputHandler {
public:
	inputHandler() = default;
	~inputHandler() = default;

	void Timer();
	

	void processInput(guiWin& app, guiWin::gui_window* window, Camera& camera);

	void mouse_callback(guiWin::gui_window* window, double xposIn, double yposIn);
	void scroll_callback(guiWin::gui_window* window, double xoffset, double yoffset);

	

private:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

};