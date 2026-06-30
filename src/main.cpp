#include <iostream>
#include <miniBoxLog.h>
#include <global.h>
#include <guiWindow.h>
#include <guiWinDeffs.h>
#include <boXM\boXM.h>
#include <boXGL.h>

// 24/06/26 A name for our graphics library is boXGL !!
int main() {

	BOX_LOG_INFO("Starting " << TITLE << " " << SCR_WIDTH << " X " << SCR_HEIGHT);

	std::cout << "Testing miniBox3d " << SCR_WIDTH << " X " << SCR_HEIGHT << " " << TITLE << std::endl;


    guiWin app;
    boXGL gl;

    guiWin::gui_window* window =
        app.guiCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE);

    if (!window)
        return -1;

    app.guiMakeContextCurrent(window);

    app.guiSetWindowIcon(window, L"icon_01.ico");
    // Default cube
    boXMesh cube =  boXCreateCubeMesh(Vec3(3.0f, 0.0f, 5.0f),  Vec3(1.5f, 1.5f, 1.5f));

    while (!app.guiWindowShouldClose(window))
    {
        app.guiPollEvents(window);

        if (app.guiGetKeys(window, GLWIN_ESCAPE) == GLWIN_PRESS) {
            std::cout << "Escape key pressed, closing window." << std::endl;
            break; // Exit loop to close window
        }
        if (app.guiGetKeys(window, GLWIN_SPACE) == GLWIN_PRESS) {
            std::cout << "Space key pressed, It works" << std::endl;
        }

        //app.guiClearColor(window, 28, 28, 28); // set the color to gray
        gl.boXGLClearColor(window, 28, 28, 28);
        gl.boXGLClearDepth(window);
       
       
        gl.boXGLDrawMesh(window, cube);


        app.guiPresent(window); // set the background color
    }

    app.guiDestroyWindow(window);
    app.guiTerminate();

    return 0;



}


//gl.boXGLDrawCube(window, Vec3(1.5f, 0.0f, 3.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 0.5f, 0.0f));

//gl.boXGLDrawLine(window, 100, 100, 600, 300, 1.0f, Vec3(1.0f, 0.0f, 0.0f));
//
//gl.boXGLDrawLine(window, 100, 150, 600, 350, 3.0f, Vec3(0.0f, 1.0f, 0.0f));
//
//gl.boXGLDrawLine(window, 100, 200, 600, 400, 5.0f, Vec3(0.0f, 0.5f, 1.0f));
//
//gl.boXGLDrawLine(window, 100, 400, 600, 400, 1.0f, Vec3(1.0f, 0.0f, 0.0f));
//
//gl.boXGLDrawCircle(window, 800, 350, 100, 2.0f, Vec3(0.0f, 1.0f, 0.0f));
//
//gl.boXGLDrawRect(window, 600, 25, 400, 200, 2.0f, Vec3(1.0f, 1.0f, 0.0f));
//gl.boXGLDrawFilledRect(window, 600, 455, 400, 200, Vec3(0.0f, 0.0f, 1.0f));