#pragma once
#include <guiWindow.h>
#include <boXM\boXM.h>
#include <mesh\boxMesh.h>
#include <camera.h> // Ensure Camera type is included

class boXGL {

public:
	
	
    // set the screen color r 0 - 255, g 0 - 255, b 0 - 255
    void boXGLClearColor(guiWin::gui_window* window, unsigned char r, unsigned char g, unsigned char b);
	// draw a pixel at (x, y) with color vec3(r, g, b) where r, g, b are in the range [0.0f, 1.0f]
    void boXGLDrawPixel(guiWin::gui_window* window, int x, int y, vec3 colour);
	// ############################################################################################################
	// ########################################### 2d Section #####################################################
	// ############################################################################################################

	// line start x sx, line start y sy, line end x ex, line end y ey    linesize  Line color
    void boXGLDrawLine(guiWin::gui_window* window, int sx, int sy, int ex, int ey, float lineSize, vec3 colour);
	// draw a circle with center at (centerX, centerY), radius, line size and color
	void boXGLDrawCircle(guiWin::gui_window* window, int centerX, int centerY, int radius, float lineSize, vec3 colour);
	// draw a rectangle with top left corner at (x, y), width and height, line size and color
	void boXGLDrawRect(guiWin::gui_window* window, int x, int y, int width, int height, float lineSize, vec3 colour);
	void boXGLDrawFilledRect(guiWin::gui_window* window, int x, int y, int width, int height, vec3 colour);
	
	// ############################################################################################################
	// ########################################### 3d Section #####################################################
	// ############################################################################################################
	struct boXScreenVertex
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	void boXGLDrawCube(guiWin::gui_window* window, vec3 position, vec3 size, vec3 colour);

	void boXGLDrawFilledTriangle(guiWin::gui_window* window, vec2 a, vec2 b, vec2 c, vec3 colour);

	void boXGLDrawPixelDepth(guiWin::gui_window* window, int x, int y, float depth, vec3 colour);

	void boXGLClearDepth(guiWin::gui_window* window);

	void boXGLDrawFilledTriangleDepth(guiWin::gui_window* window, boXScreenVertex a, boXScreenVertex b, boXScreenVertex c, vec3 colour);
	
	void boXGLClearDepthBuffer(guiWin::gui_window* window); // move to boXGL class

	void boXGLDrawMeshFaces(guiWin::gui_window* window, const Camera& camera, const boXMesh& mesh);
	
	void boXGLDrawMeshEdges(guiWin::gui_window* window, const Camera& camera, const boXMesh& mesh, vec3 colour);
	
	void boXGLDrawMeshVertices(guiWin::gui_window* window, const Camera& camera, const boXMesh& mesh, vec3 colour);
	
	void boXGLDrawMesh(guiWin::gui_window* window, const Camera& camera, const boXMesh& mesh);

private:
};
