#pragma once
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <vector>
#include <cstdint>



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

		BITMAPINFO bitmapInfo{};
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

	void guiSwapBuffers(gui_window* window);
	
	
	// Window icon 
	void guiSetWindowIcon(gui_window* window, const wchar_t* iconPath);
	
	typedef void(*GLwinResizeCallback)(int width, int height);

	
	// Keyboard input API
	//int guiGetKey(gui_window* window, int keycode);

	// Mouse input API
	//int guiGetMouseButton(gui_window* window, int button);
	//void guiGetCursorPos(gui_window* window, double* xpos, double* ypos);

	// guiUI: Simple GUI message box
	// guiUI_MessageBox(const wchar_t* title, const wchar_t* message);

	
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

