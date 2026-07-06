#include "editorRender.h"

void editorRender::DrawEditorOverlays(
    guiWin::gui_window* window,
    boXGL& gl,
    const Camera& camera,
    const EditorState& editor,
    const boXMesh& cube,
    const vec3& cubePosition,
    bool cubeSelected)
{
    if (!window)
        return;

    if (editor.mode == EditorMode::ObjectMode)
    {
        if (cubeSelected)
        {
            gl.boXGLDrawOriginMarker(
                window,
                camera,
                cubePosition,
                vec3(1.0f, 0.9f, 0.1f),
                10);
        }
    }
    /*else if (editor.mode == EditorMode::EditMode)
    {
        if (editor.selectMode == EditSelectMode::Vertex)
        {
            gl.boXGLDrawMeshVertices(
                window,
                camera,
                cube,
                vec3(0.2f, 0.8f, 1.0f));

            gl.boXGLDrawMeshEdges(
                window,
                camera,
                cube,
                vec3(0.05f, 0.05f, 0.05f));
        }
    }*/

    //#####
    else if (editor.mode == EditorMode::EditMode)
    {
        if (editor.selectMode == EditSelectMode::Vertex)
        {
            gl.boXGLDrawMeshEdges(
                window,
                camera,
                cube,
                vec3(0.05f, 0.05f, 0.05f));

            gl.boXGLDrawMeshVertices(
                window,
                camera,
                cube,
                vec3(1.0f, 0.45f, 0.1f)); // soft orange 
                //vec3(0.2f, 0.8f, 1.0f));

            if (editor.selectedVertex >= 0 &&
                editor.selectedVertex < static_cast<int>(cube.vertices.size()))
            {
                const vec3& pos = cube.vertices[editor.selectedVertex].position;

                gl.boXGLDrawOriginMarker(
                    window,
                    camera,
                    pos,
                    vec3(0.0f, 1.5f, 0.0f),
                    3);
            }
        }
        else if (editor.selectMode == EditSelectMode::Edge)
        {
            gl.boXGLDrawMeshEdges(
                window,
                camera,
                cube,
                vec3(1.0f, 0.5f, 0.0f)); //orange

            if (editor.selectedEdge >= 0 &&
                editor.selectedEdge < static_cast<int>(cube.edges.size()))
            {
                const boXEdge& edge = cube.edges[editor.selectedEdge];

                gl.boXGLDrawLine3D(
                    window,
                    camera,
                    cube.vertices[edge.v0].position,
                    cube.vertices[edge.v1].position,
                    1.0f,
                    vec3(0.0f, 0.5f, 1.0f));
            }



        }
        else if (editor.selectMode == EditSelectMode::Face)
        {
            // We can add face highlighting later.
            gl.boXGLDrawMeshEdges(
                window,
                camera,
                cube,
                vec3(0.05f, 0.05f, 0.05f));
        }
    }

}