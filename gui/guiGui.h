#pragma once
#include <guiWindow.h>


class guiGui {
public:

	guiGui();
	~guiGui();
	void guiInit(guiWin::gui_window* window);
	void guiNewFrame(guiWin::gui_window* window);
	void guiDockSpace(bool* p_open);
	void guiRender(guiWin::gui_window* window);


	void guiGuiCleanUp();


};
