#include <iostream>
#include <miniBoxLog.h>
#include <guiWindow.h>
#include <guiWinDeffs.h>
#include <boXM\boXM.h>
#include <boXGL.h>
#include <camera.h>
#include <editorInput.h>
#include <editorState.h>
#include <guiUI.h>
#include <editorRender.h>
#include <sceneObject.h>
#include <vector>
#include <boXImage.h>


// 24/06/26 A name for our graphics library is boXGL !!
// 1. object mode edit mode improvment object mode no vertex / edge / face selection just object selection
// 2. improve selection
// 3. right click context menu for selection and edit mode
// 4. Then move to vertex / edge / face selection using 1,2,3 to select
// 5. scene collection
// 6. Improved Render


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
    EditorState editor;
	editorRender renderEditor;
    boXImage img;
   
    guiWin::gui_window* window =
        app.guiCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE);

    if (!window)
        return -1;

    app.guiMakeContextCurrent(window);

    app.guiSetWindowIcon(window, L"icon_01.ico");

    // munu icons
    boXImg selectIcon;
    boXImg moveIcon;
    boXImg scaleIcon;
    boXImg rotateIcon;
    boXImg extrudeIcon;
    // C:\Users\marty\Desktop\minibox 3d\MiniBoxEditor\icons
    img.boXLoadImage("C:/Users/marty/Desktop/minibox 3d/MiniBoxEditor/icons/select.png", selectIcon);
    img.boXLoadImage("C:/Users/marty/Desktop/minibox 3d/MiniBoxEditor/icons/scale_L.png", scaleIcon);
    img.boXLoadImage("C:/Users/marty/Desktop/minibox 3d/MiniBoxEditor/icons/move_L.png", moveIcon);
    img.boXLoadImage("C:/Users/marty/Desktop/minibox 3d/MiniBoxEditor/icons/rotate.png", rotateIcon);
   // boXLoadImage("icons/rotate.png", rotateIcon);
   // boXLoadImage("icons/extrude.png", extrudeIcon);


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
    boXMesh cube = boXCreateCubeMesh(cubePosition, vec3(1.5f, 1.5f, 1.5f));



	// ################################## Scene collection / object list ################
    std::vector<SceneObject> sceneObjects;

    SceneObject cubeSceneObject;
    cubeSceneObject.id = 0;
    cubeSceneObject.name = "Cube";
    cubeSceneObject.type = SceneObjectType::Mesh;
    cubeSceneObject.position = cubePosition;
    cubeSceneObject.rotation = vec3(0.0f, 0.0f, 0.0f);
    cubeSceneObject.scale = vec3(1.5f, 1.5f, 1.5f);
    cubeSceneObject.selected = false;
    cubeSceneObject.expanded = false;

    sceneObjects.push_back(cubeSceneObject);

    int selectedSceneObject = -1;
    // ##################################


    

    while (!app.guiWindowShouldClose(window))
    {
		in.Timer();

        app.guiPollEvents(window);
              
		in.processInput(app, window, camera);// process keyboard & mouse input for camera movement
        in.editMode(app, window, editor); // process keyboard TAB for edit mode / object mode selection
        in.editSelection(app, window, gl, camera, editor, cube);
		in.editCubeSelected(app, window, gl, camera, editor, cubePosition, cubeSelected);
        
        // set the color to gray
        gl.boXGLClearColor(window, 28, 28, 28);
		// clear the depth buffer
        gl.boXGLClearDepth(window);     
        
		gl.boXGLDrawGrid(window, camera, 20, 1.0f); // draw a grid with size 20 and spacing 1.0f

        // draw a default cube and look at it with the new camera
        gl.boXGLDrawMesh(window, camera, cube);
        
		// draw editor overlays based on the current editor state
		renderEditor.DrawEditorOverlays(window, gl, camera, editor, cube, cubePosition, cubeSelected);
        
        ui.UInewFrame(app, window, gl);

		// ###################################### scene collection / object list ################
        static bool sceneWindowOpen = true;

		ui.UISetNextWindowSize(300.0f, 400.0f);
		ui.UISetNextWindowPos(SCR_WIDTH - 300, 28.0f);
        ui.UIbegin("Scene Collection", &sceneWindowOpen, 2);

        float rowY = 45.0f;

        for (int i = 0; i < static_cast<int>(sceneObjects.size()); ++i)
        {
            bool clicked =
                ui.WidgetTreeNode(
                    sceneObjects[i].name.c_str(),
                    sceneObjects[i].expanded,
                    selectedSceneObject == i,
                    12.0f,
                    rowY,
                    260.0f,
                    26.0f);

            if (clicked)
            {
                selectedSceneObject = i;

                for (auto& obj : sceneObjects)
                    obj.selected = false;

                sceneObjects[i].selected = true;

                if (sceneObjects[i].name == "Cube")
                {
                    cubeSelected = true;
                }
            }

            rowY += 28.0f;
        }

        ui.End();
		// ###################################### End scene collection / object list ################
        ui.UIFrame(10.0f, 30.0f, 42.0f, 500.0f, SIDE_BAR_MAIN); // Side bar menu

        if (ui.WidgetSideImageButton(selectIcon, editor.activeTool == EditorTool::None))
        {
            editor.activeTool = EditorTool::None;
            BOX_LOG_INFO("Select Tool");
        }

        if (ui.WidgetSideImageButton(scaleIcon, editor.activeTool == EditorTool::Scale))
        {
            editor.activeTool = EditorTool::Scale;
            BOX_LOG_INFO("Scale Tool");
        }

        if (ui.WidgetSideImageButton(moveIcon, editor.activeTool == EditorTool::Move))
        {
            editor.activeTool = EditorTool::Move;
            BOX_LOG_INFO("Move Tool");
        }

        if (ui.WidgetSideImageButton(rotateIcon, editor.activeTool == EditorTool::Rotate))
        {
            editor.activeTool = EditorTool::Rotate;
            BOX_LOG_INFO("Rotate Tool");
        }

        

        /*if (ui.WidgetSideButton("M", editor.activeTool == EditorTool::Move))
        {
            BOX_LOG_INFO("Move Tool");
            editor.activeTool = EditorTool::Move;
        }

        if (ui.WidgetSideButton("R", editor.activeTool == EditorTool::Rotate))
        {
            BOX_LOG_INFO("Rotate Tool");
            editor.activeTool = EditorTool::Rotate;
        }

        if (ui.WidgetSideButton("E", editor.activeTool == EditorTool::Extrude))
        {
            BOX_LOG_INFO("Extrude Tool");
            editor.activeTool = EditorTool::Extrude;
        }

        if (ui.WidgetSideButton("G", editor.activeTool == EditorTool::Grab))
        {
            editor.activeTool = EditorTool::Grab;
        }*/

        ui.UIFrameEnd();
        
        ui.UIFrame(0.0f, 0.0f, SCR_WIDTH, 28.0f, MENU_BAR_MAIN); // set to the screen width top menu


        if (ui.WidgetFrameButton("File", 10.0f, 2.0f, 60.0f, BUTTON_SIZE_SMALL))
        {
            BOX_LOG_INFO("File clicked");
        }

        if (ui.WidgetFrameButton("Edit", 71.0f, 2.0f, 60.0f, BUTTON_SIZE_SMALL))
        {
            BOX_LOG_INFO("Edit clicked");
        }

        if (ui.WidgetFrameButton("Render", 132.0f, 2.0f, 60.0f, BUTTON_SIZE_SMALL))
        {
            BOX_LOG_INFO("RRender clicked");
        }
        if (ui.WidgetFrameButton("Add", 194.0f, 2.0f, 60.0f, BUTTON_SIZE_SMALL))
        {
            BOX_LOG_INFO("Add Object clicked");
        }

        ui.UIFrameEnd();

        /*ui.UISetNextWindowSize(SCR_WIDTH, 28.0f);
        ui.UISetNextWindowPos(0.0f, 0.0f);
		ui.UIbegin("Menu Bar", nullptr, 3);
        ui.WidgetMenuBar({ "File", "Edit", "View", "Help", "About"}, 0.0f, 0.0f, static_cast<float>(SCR_WIDTH - 300), 28.0f);
		ui.End();*/

        static bool demoWindowOpen = true;
        ui.UISetNextWindowSize(300.0f, 300.0f);
        ui.UISetNextWindowPos(SCR_WIDTH - 300, 428.0f);
        ui.UIbegin("Properties ", &demoWindowOpen, 1);

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