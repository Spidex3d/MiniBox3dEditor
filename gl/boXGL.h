#pragma once
#include <guiWindow.h>
#include <boXM\boXM.h>

class boXGL {

public:
	
    // set the screen color r 0 - 255, g 0 - 255, b 0 - 255
    void boXGLClearColor(guiWin::gui_window* window, unsigned char r, unsigned char g, unsigned char b);

    void boXGLDrawPixel(guiWin::gui_window* window, int x, int y, Vec3 colour);

	// line start x sx, line start y sy, line end x ex, line end y ey    linesize  Line color
    void boXGLDrawLine(guiWin::gui_window* window, int sx, int sy, int ex, int ey, float lineSize, Vec3 colour);

	void boXGLDrawRect(guiWin::gui_window* window, int x, int y, int width, int height, float lineSize, Vec3 colour);



private:
};
