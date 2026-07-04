#include <iostream>
#include <miniBoxLog.h>
#include <guiWindow.h>
#include <guiWinDeffs.h>
#include <boXM\boXM.h>
#include <boXGL.h>
#include <camera.h>
#include <editorInput.h>
#include <guiUI.h>


// 24/06/26 A name for our graphics library is boXGL !!
// 1. Object Mode selection first
// 2. Click near projected object origin or bounding box
// 3. Highlight selected object
// 4. Then move to vertex / edge / face selection
// 5. Start a simpel UI
// 6. Improve Render


// guiWin = OS window / input
// boXGL = software renderer
// Camera = editor camera
// editorInput = camera / editor input
// guiUI = editor UI widgets
// main.cpp = connects everything

int main() {

	BOX_LOG_INFO("Starting " << TITLE << " " << SCR_WIDTH << " X " << SCR_HEIGHT);

	std::cout << "Testing miniBox3d " << SCR_WIDTH << " X " << SCR_HEIGHT << " " << TITLE << std::endl;


    guiWin app;
    boXGL gl;
    inputHandler in;
    guiUI ui;
	

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
    bool cubeSelected = false;
    vec3 cubePosition = vec3(0.0f, 0.75f, 0.0f);
    boXMesh cube =  boXCreateCubeMesh(cubePosition,  vec3(1.5f, 1.5f, 1.5f));

    while (!app.guiWindowShouldClose(window))
    {
		in.Timer();

        app.guiPollEvents(window);
              
		in.processInput(app, window, camera);// process keyboard & mouse input for camera movement

        if (app.guiGetMouseButtonPressed(window, GLWIN_MOUSE_BUTTON_LEFT) == GLWIN_PRESS)
        {
            double mx = 0.0;
            double my = 0.0;

            app.guiGetCursorPos(window, &mx, &my);

            boXGL::boXScreenVertex originScreen;

            if (gl.boXGLProjectWorldToScreen(window, camera, cubePosition, originScreen))
            {
                float dx = static_cast<float>(mx) - originScreen.x;
                float dy = static_cast<float>(my) - originScreen.y;

                float distanceSquared = dx * dx + dy * dy;

                float pickRadius = 18.0f;

                if (distanceSquared <= pickRadius * pickRadius)
                {
                    cubeSelected = true;
                    BOX_LOG_INFO("Cube selected");
                }
                else
                {
                    cubeSelected = false;
                    BOX_LOG_INFO("Selection cleared");
                }
            }
        }

        // set the color to gray
        gl.boXGLClearColor(window, 28, 28, 28);
		// clear the depth buffer
        gl.boXGLClearDepth(window);     
        
		gl.boXGLDrawGrid(window, camera, 20, 1.0f); // draw a grid with size 20 and spacing 1.0f

        // draw a default cube and look at it with the new camera
        gl.boXGLDrawMesh(window, camera, cube);
        //gl.boXGLDrawOriginMarker(window, camera, vec3(0.0f, 0.75f, 0.0f));
        gl.boXGLDrawOriginMarker(window, camera, cubePosition, cubeSelected ? vec3(1.0f, 0.9f, 0.1f) : vec3(1.0f, 0.7f, 0.1f),
            cubeSelected ? 10 : 6);

        

        ui.UInewFrame(app, window, gl);

        static bool demoWindowOpen = true;

        ui.UIbegin("Demo Window", &demoWindowOpen, 1);

        if (ui.WidgetButton("OK", 20.0f, 50.0f, 80.0f, 28.0f))
        {
            BOX_LOG_INFO("OK button clicked");
        }

        ui.End();

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