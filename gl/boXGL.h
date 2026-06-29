#pragma once
#include <guiWindow.h>
#include <boXM\boXM.h>
#include <mesh\boxMesh.h>

class boXGL {

public:
	
    // set the screen color r 0 - 255, g 0 - 255, b 0 - 255
    void boXGLClearColor(guiWin::gui_window* window, unsigned char r, unsigned char g, unsigned char b);

    void boXGLDrawPixel(guiWin::gui_window* window, int x, int y, Vec3 colour);
    // 2d section
	// line start x sx, line start y sy, line end x ex, line end y ey    linesize  Line color
    void boXGLDrawLine(guiWin::gui_window* window, int sx, int sy, int ex, int ey, float lineSize, Vec3 colour);
	// draw a circle with center at (centerX, centerY), radius, line size and color
	void boXGLDrawCircle(guiWin::gui_window* window, int centerX, int centerY, int radius, float lineSize, Vec3 colour);
	// draw a rectangle with top left corner at (x, y), width and height, line size and color
	void boXGLDrawRect(guiWin::gui_window* window, int x, int y, int width, int height, float lineSize, Vec3 colour);
	void boXGLDrawFilledRect(guiWin::gui_window* window, int x, int y, int width, int height, Vec3 colour);
	
	// 3d section
	void boXGLDrawCube(guiWin::gui_window* window, Vec3 position, Vec3 size, Vec3 colour);

	void boXGLDrawFilledTriangle(guiWin::gui_window* window, Vec2 a, Vec2 b, Vec2 c, Vec3 colour);
	
	void boXGLDrawMeshFaces(guiWin::gui_window* window,	const boXMesh& mesh);
	
	void boXGLDrawMeshEdges(guiWin::gui_window* window,	const boXMesh& mesh, Vec3 colour);
	
	void boXGLDrawMeshVertices(guiWin::gui_window* window, const boXMesh& mesh,	Vec3 colour);
	
	void boXGLDrawMesh(guiWin::gui_window* window, const boXMesh& mesh);

private:
};
