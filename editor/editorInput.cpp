#include "editorInput.h"
#include <guiWindow.h>
#include <guiWinDeffs.h>
#include <miniBoxLog.h>



bool firstMouse = true;
bool mouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float fov = 45.0f;

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

void inputHandler::editMode(guiWin& app, guiWin::gui_window* window, EditorState& editor)
{
    // TAB switches between Object Mode and Edit Mode
    if (app.guiGetKeyPressed(window, GLWIN_TAB) == GLWIN_PRESS)
    {
        if (editor.mode == EditorMode::ObjectMode)
        {
            editor.mode = EditorMode::EditMode;
            editor.selectMode = EditSelectMode::Vertex;

            BOX_LOG_INFO("Edit Mode");
            BOX_LOG_INFO("Vertex Mode");
        }
        else
        {
            editor.mode = EditorMode::ObjectMode;

            BOX_LOG_INFO("Object Mode");
        }
    }

    // 1, 2, 3 only work while in Edit Mode
    if (editor.mode == EditorMode::EditMode)
    {
        if (app.guiGetKeyPressed(window, GLWIN_KEY_1) == GLWIN_PRESS)
        {
            editor.selectMode = EditSelectMode::Vertex;
            BOX_LOG_INFO("Vertex Mode");
        }

        if (app.guiGetKeyPressed(window, GLWIN_KEY_2) == GLWIN_PRESS)
        {
            editor.selectMode = EditSelectMode::Edge;
            BOX_LOG_INFO("Edge Mode");
        }

        if (app.guiGetKeyPressed(window, GLWIN_KEY_3) == GLWIN_PRESS)
        {
            editor.selectMode = EditSelectMode::Face;
            BOX_LOG_INFO("Face Mode");
        }
    }
}

void inputHandler::editCubeSelected(guiWin& app, guiWin::gui_window* window, boXGL& gl, const Camera& camera, EditorState& editor, const vec3& cubePosition, bool& cubeSelected)
{
    if (editor.mode != EditorMode::ObjectMode)
        return;

    if (app.guiGetMouseButtonPressed(window, GLWIN_MOUSE_BUTTON_LEFT) != GLWIN_PRESS)
        return;

    double mx = 0.0;
    double my = 0.0;

    app.guiGetCursorPos(window, &mx, &my);

    boXGL::boXScreenVertex originScreen;

    if (gl.boXGLProjectWorldToScreen(
        window,
        camera,
        cubePosition,
        originScreen))
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

int inputHandler::PickVertex(guiWin& app, guiWin::gui_window* window, boXGL& gl, const Camera& camera, const boXMesh& mesh, float pickRadius)
{
    double mx = 0.0;
    double my = 0.0;

    app.guiGetCursorPos(window, &mx, &my);

    int bestIndex = -1;
    float bestDistanceSq = pickRadius * pickRadius;

    for (int i = 0; i < static_cast<int>(mesh.vertices.size()); ++i)
    {
        boXGL::boXScreenVertex p;

        if (!gl.boXGLProjectWorldToScreen(
            window,
            camera,
            mesh.vertices[i].position,
            p))
        {
            continue;
        }

        float dx = static_cast<float>(mx) - p.x;
        float dy = static_cast<float>(my) - p.y;

        float distanceSq = dx * dx + dy * dy;

        if (distanceSq < bestDistanceSq)
        {
            bestDistanceSq = distanceSq;
            bestIndex = i;
        }
    }

    return bestIndex;
}
static float DistancePointToLineSegmentSq(
    float px,
    float py,
    float ax,
    float ay,
    float bx,
    float by)
{
    float abx = bx - ax;
    float aby = by - ay;

    float apx = px - ax;
    float apy = py - ay;

    float abLenSq = abx * abx + aby * aby;

    if (abLenSq <= 0.0001f)
        return apx * apx + apy * apy;

    float t = (apx * abx + apy * aby) / abLenSq;

    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    float closestX = ax + abx * t;
    float closestY = ay + aby * t;

    float dx = px - closestX;
    float dy = py - closestY;

    return dx * dx + dy * dy;
}

int inputHandler::PickEdge(guiWin& app, guiWin::gui_window* window, boXGL& gl, const Camera& camera, const boXMesh& mesh, float pickRadius)
{
    double mx = 0.0;
    double my = 0.0;

    app.guiGetCursorPos(window, &mx, &my);

    int bestIndex = -1;
    float bestDistanceSq = pickRadius * pickRadius;

    for (int i = 0; i < static_cast<int>(mesh.edges.size()); ++i)
    {
        const boXEdge& edge = mesh.edges[i];

        boXGL::boXScreenVertex a;
        boXGL::boXScreenVertex b;

        if (!gl.boXGLProjectWorldToScreen(
            window,
            camera,
            mesh.vertices[edge.v0].position,
            a))
        {
            continue;
        }

        if (!gl.boXGLProjectWorldToScreen(
            window,
            camera,
            mesh.vertices[edge.v1].position,
            b))
        {
            continue;
        }

        float distanceSq =
            DistancePointToLineSegmentSq(
                static_cast<float>(mx),
                static_cast<float>(my),
                a.x,
                a.y,
                b.x,
                b.y);

        if (distanceSq < bestDistanceSq)
        {
            bestDistanceSq = distanceSq;
            bestIndex = i;
        }
    }

    return bestIndex;
}


void inputHandler::editSelection(guiWin& app, guiWin::gui_window* window, boXGL& gl, const Camera& camera, EditorState& editor, const boXMesh& mesh)
{
    if (editor.mode != EditorMode::EditMode)
        return;

    if (app.guiGetMouseButtonPressed(window, GLWIN_MOUSE_BUTTON_LEFT) != GLWIN_PRESS)
        return;

    if (editor.selectMode == EditSelectMode::Vertex)
    {
        int pickedVertex =
            PickVertex(
                app,
                window,
                gl,
                camera,
                mesh,
                12.0f);

        editor.selectedVertex = pickedVertex;
        editor.selectedEdge = -1;
        editor.selectedFace = -1;

        if (pickedVertex >= 0)
        {
            BOX_LOG_INFO("Selected vertex: " << pickedVertex);
        }
        else
        {
            BOX_LOG_INFO("No vertex selected");
        }
    }
    else if (editor.selectMode == EditSelectMode::Edge)
    {
        int pickedEdge =
            PickEdge(
                app,
                window,
                gl,
                camera,
                mesh,
                10.0f);

        editor.selectedVertex = -1;
        editor.selectedEdge = pickedEdge;
        editor.selectedFace = -1;

        if (pickedEdge >= 0)
        {
            BOX_LOG_INFO("Selected edge: " << pickedEdge);
        }
        else
        {
            BOX_LOG_INFO("No edge selected");
        }
    }
}



