#include <iostream>
#include <miniBoxLog.h>
#include <global.h>
#include <guiWindow.h>
#include <guiWinDeffs.h>
#include <boXM\boXM.h>
#include <boXGL.h>
#include <editorState.h>

// 24/06/26 A name for our graphics library is boXGL !!

int main() {

	BOX_LOG_INFO("Starting " << TITLE << " " << SCR_WIDTH << " X " << SCR_HEIGHT);

	std::cout << "Testing miniBox3d " << SCR_WIDTH << " X " << SCR_HEIGHT << " " << TITLE << std::endl;


    guiWin app;
    boXGL gl;
	
	EditorState editor; // used for selecting between object mode and edit mode

    guiWin::gui_window* window =
        app.guiCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE);

    if (!window)
        return -1;

    app.guiMakeContextCurrent(window);

    app.guiSetWindowIcon(window, L"icon_01.ico");
    // Default cube
    boXMesh cube =  boXCreateCubeMesh(vec3(3.0f, 0.0f, 5.0f),  vec3(1.5f, 1.5f, 1.5f));

    while (!app.guiWindowShouldClose(window))
    {
        app.guiPollEvents(window);

        if (app.guiGetKeyPressed(window, GLWIN_TAB) == GLWIN_PRESS)
        {
            if (editor.mode == EditorMode::ObjectMode)
            {
                editor.mode = EditorMode::EditMode;
                BOX_LOG_INFO("Edit Mode");
            }
            else
            {
                editor.mode = EditorMode::ObjectMode;
                BOX_LOG_INFO("Object Mode");
            }
        }

		
        // mouse input
        double mx, my;
        app.guiGetCursorPos(window, &mx, &my);
        if (app.guiGetMouseButton(window, GLWIN_MOUSE_BUTTON_LEFT) == GLWIN_PRESS) {
            BOX_LOG_INFO("Mouse position: (" << mx << ", " << my << ")");
        }

        //app.guiClearColor(window, 28, 28, 28); // set the color to gray
        gl.boXGLClearColor(window, 28, 28, 28);
        gl.boXGLClearDepth(window);
       
       
        gl.boXGLDrawMesh(window, cube);


        app.guiPresent(window); // set the background color

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