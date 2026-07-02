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
//void inputHandler::processInput(guiWin::gui_window* window)
{
  
    if (app.guiGetKeys(window, GLWIN_KEY_W) == GLWIN_PRESS)
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
    }
    // UP - Down
    if (app.guiGetKeys(window, GLWIN_KEY_Z) == GLWIN_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (app.guiGetKeys(window, GLWIN_KEY_X) == GLWIN_PRESS)
        camera.ProcessKeyboard(UP, -deltaTime);


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
}



void inputHandler::mouse_callback(guiWin::gui_window* window, double xposIn, double yposIn)
{
}

void inputHandler::scroll_callback(guiWin::gui_window* window, double xoffset, double yoffset)
{
}
