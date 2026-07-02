#include "editorInput.h"
#include <guiWindow.h>
#include <guiWinDeffs.h>
#include <miniBoxLog.h>
#include <editorState.h>


//Camera camera(vec3(0.0f, 0.0f, 5.0f));

bool firstMouse = true;
bool mouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float fov = 45.0f;

EditorState editor; // used for selecting between object mode and edit mode

void inputHandler::Timer()
{
      
        float currentFrame = static_cast<float>(GetTickCount64()) * 0.001f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    
}
void inputHandler::processInput(guiWin& app, guiWin::gui_window* window, Camera& camera)
{
  
    /*if (app.guiGetKeys(window, GLWIN_KEY_W) == GLWIN_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (app.guiGetKeys(window, GLWIN_KEY_S) == GLWIN_PRESS)
    { 
        
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (app.guiGetKeys(window, GLWIN_KEY_A) == GLWIN_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (app.guiGetKeys(window, GLWIN_KEY_D) == GLWIN_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }*/
    // UP - Down
    /*if (app.guiGetKeys(window, GLWIN_KEY_Z) == GLWIN_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (app.guiGetKeys(window, GLWIN_KEY_X) == GLWIN_PRESS)
        camera.ProcessKeyboard(UP, -deltaTime);*/


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



    double dx = 0.0;
    double dy = 0.0;

    app.guiGetMouseDelta(window, &dx, &dy);

    if (app.guiGetMouseButton(window, GLWIN_MOUSE_BUTTON_MIDDLE) == GLWIN_PRESS)
    {
        bool middleMouseDown =
            app.guiGetMouseButton(window, GLWIN_MOUSE_BUTTON_MIDDLE) == GLWIN_PRESS;

        bool shiftDown =
            app.guiGetKeys(window, GLWIN_SHIFT) == GLWIN_PRESS;

        if (middleMouseDown && shiftDown)
        {
            camera.ProcessOrbitPan(
                static_cast<float>(dx),
                static_cast<float>(dy));
        }
        else if (middleMouseDown)
        {
            camera.ProcessOrbit(
                static_cast<float>(dx),
                static_cast<float>(-dy));
        }
       
    }

    int wheel = app.guiGetMouseWheel(window);

    if (wheel != 0)
    {
        camera.ProcessOrbitZoom(static_cast<float>(wheel));
    }

}



