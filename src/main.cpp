#include <iostream>
#include <miniBoxLog.h>
#include <guiWindow.h>
#include <guiWinDeffs.h>
#include <boXM\boXM.h>
#include <boXGL.h>
#include <camera.h>
#include <editorInput.h>

// 24/06/26 A name for our graphics library is boXGL !!
// 1. Object Mode selection first
// 2. Click near projected object origin or bounding box
// 3. Highlight selected object
// 4. Then move to vertex / edge / face selection//4. Then selection

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
    camera.Target = vec3(0.0f, 4.0f, -5.0f);
    camera.Distance = 20.0f;
    //camera.Yaw = 90.0f;
    camera.Yaw = 45.0f;
    camera.Pitch = -25.0f;
    camera.UpdateOrbitCamera();

    // Default cube
    boXMesh cube =  boXCreateCubeMesh(vec3(0.0f, 0.75f, 0.0f),  vec3(1.5f, 1.5f, 1.5f));

    while (!app.guiWindowShouldClose(window))
    {
		in.Timer();

        app.guiPollEvents(window);
              
		in.processInput(app, window, camera);// process keyboard & mouse input for camera movement

        // set the color to gray
        gl.boXGLClearColor(window, 28, 28, 28);
		// clear the depth buffer
        gl.boXGLClearDepth(window);     
        
		gl.boXGLDrawGrid(window, camera, 20, 1.0f); // draw a grid with size 20 and spacing 1.0f
        // draw a default cube and look at it with the new camera
        gl.boXGLDrawMesh(window, camera, cube);
        gl.boXGLDrawOriginMarker(window, camera, vec3(0.0f, 0.75f, 0.0f));

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