#pragma once
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <vector>
#include <cstdint>
//#include <unordered_map>
#include <boXM\boXM.h>




class guiWin {

public:
	struct gui_window
	{
		HWND hwnd = nullptr;
		HINSTANCE instance = nullptr;

		int width = 0;
		int height = 0;

		bool shouldClose = false;
		// Our software screen buffer  
		std::vector<unsigned int> pixels;

		// depth buffer  
		std::vector<float> depthBuffer;

		BITMAPINFO bitmapInfo{};

		// Keyboard state  
		bool keys[256] = {};
		bool keysPressed[256] = {};
		bool keysReleased[256] = {};

		// Mouse state  
		double mouseX = 0.0;
		double mouseY = 0.0;

		double lastMouseX = 0.0;
		double lastMouseY = 0.0;

		double mouseDeltaX = 0.0;
		double mouseDeltaY = 0.0;

		bool mouseButtons[3] = {};
		bool mouseButtonsPressed[3] = {};
		bool mouseButtonsReleased[3] = {};

		int mouseWheelDelta = 0;
		
		// User pointer for custom data  
		void* userPointer = nullptr;
		
		/*std::unordered_map<int, bool> keyState;
		std::unordered_map<int, bool> prevKeyState;*/

		//// mouse state
		//double mouseX = 0.0, mouseY = 0.0;
		//bool mouseButtons[3] = { false, false, false };
	};
	
public:
	guiWin();
	~guiWin();

	gui_window* guiCreateWindow(
		int width,
		int height,
		const wchar_t* title);

	// Window/context management
	void guiMakeContextCurrent(gui_window* window);

	bool guiWindowShouldClose(gui_window* window);

	void guiPollEvents(gui_window* window);

	void guiPresent(gui_window* window); //send the color to the screen
	
	// Window icon 
	void guiSetWindowIcon(gui_window* window, const wchar_t* iconPath);
	
	typedef void(*GLwinResizeCallback)(int width, int height);

	// ################################################################################################################
	// ############################################ Keyboard and mouse input ##########################################
	// ################################################################################################################
	int guiGetKeys(gui_window* window, int keycode);
	int guiGetKeyPressed(gui_window* window, int keycode);
	int guiGetKeyReleased(gui_window* window, int keycode);

	void guiGetCursorPos(gui_window* window, double* xpos, double* ypos);

	void guiGetMouseDelta(gui_window* window, double* dx, double* dy);
	int guiGetMouseWheel(gui_window* window);
	int guiGetMouseButton(gui_window* window, int button);
	
	int guiGetMouseButtonPressed(gui_window* window, int button);
	int guiGetMouseButtonReleased(gui_window* window, int button);


	void guiEndFrame(gui_window* window);
	
	





	
	// Terminate and cleanup library 
	void guiDestroyWindow(gui_window* window);

	void guiTerminate();


private:
	static LRESULT CALLBACK WindowProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);


};

