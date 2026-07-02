#include <iostream>
#include <miniBoxLog.h>
#include <guiWindow.h>
#include <guiWinDeffs.h>
#include <boXM\boXM.h>
#include <boXGL.h>
#include <camera.h>
#include <editorInput.h>

// 24/06/26 A name for our graphics library is boXGL !!

int main() {

	BOX_LOG_INFO("Starting " << TITLE << " " << SCR_WIDTH << " X " << SCR_HEIGHT);

	std::cout << "Testing miniBox3d " << SCR_WIDTH << " X " << SCR_HEIGHT << " " << TITLE << std::endl;


    guiWin app;
    boXGL gl;
    inputHandler in;
	

    guiWin::gui_window* window =
        app.guiCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE);

    if (!window)
        return -1;

    app.guiMakeContextCurrent(window);

    app.guiSetWindowIcon(window, L"icon_01.ico");

    Camera camera(vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 1.0f, 0.0f));
    camera.Target = vec3(3.0f, 0.0f, 5.0f);
    camera.Distance = 6.0f;
    camera.Yaw = 90.0f;
    camera.Pitch = 0.0f;
    camera.UpdateOrbitCamera();

    // Default cube
    boXMesh cube =  boXCreateCubeMesh(vec3(3.0f, 0.0f, 5.0f),  vec3(1.5f, 1.5f, 1.5f));

    while (!app.guiWindowShouldClose(window))
    {
		in.Timer();

        app.guiPollEvents(window);
              
		in.processInput(app, window, camera);// process keyboard input for camera movement

		
        // mouse input
        double mx, my;
        app.guiGetCursorPos(window, &mx, &my);
        if (app.guiGetMouseButton(window, GLWIN_MOUSE_BUTTON_LEFT) == GLWIN_PRESS) {
            BOX_LOG_INFO("Mouse position: (" << mx << ", " << my << ")");
        }

        // set the color to gray
        gl.boXGLClearColor(window, 28, 28, 28);
		// clear the depth buffer
        gl.boXGLClearDepth(window);     
        

        // draw a cube and look at it with the new camera
        gl.boXGLDrawMesh(window, camera, cube);

		app.guiPresent(window); // set the background color and draw the cube

        app.guiEndFrame(window);
    }

    app.guiDestroyWindow(window);
    app.guiTerminate();

    return 0;



}


//gl.boXGLDrawCube(window, vec3(1.5f, 0.0f, 3.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 0.5f, 0.0f));

//gl.boXGLDrawLine(window, 100, 100, 600, 300, 1.0f, vec3(1.0f, 0.0f, 0.0f));
//
//gl.boXGLDrawLine(window, 100, 150, 600, 350, 3.0f, vec3(0.0f, 1.0f, 0.0f));
//
//gl.boXGLDrawLine(window, 100, 200, 600, 400, 5.0f, vec3(0.0f, 0.5f, 1.0f));
//
//gl.boXGLDrawLine(window, 100, 400, 600, 400, 1.0f, vec3(1.0f, 0.0f, 0.0f));
//
//gl.boXGLDrawCircle(window, 800, 350, 100, 2.0f, vec3(0.0f, 1.0f, 0.0f));
//
//gl.boXGLDrawRect(window, 600, 25, 400, 200, 2.0f, vec3(1.0f, 1.0f, 0.0f));
//gl.boXGLDrawFilledRect(window, 600, 455, 400, 200, vec3(0.0f, 0.0f, 1.0f));