#pragma once

enum class EditorMode
{
    ObjectMode,
    EditMode
};
//EditorMode editorMode = EditorMode::ObjectMode;

enum class EditSelectMode
{
    Vertex,
    Edge,
    Face
};

enum class EditorTool
{
    None,
    Move,
    Grab,
    Rotate,
    Scale,
    Extrude
};


struct EditorState
{
    EditorMode mode = EditorMode::ObjectMode;
    EditSelectMode selectMode = EditSelectMode::Vertex;
    EditorTool activeTool = EditorTool::None;

    int selectedVertex = -1;
    int selectedEdge = -1;
    int selectedFace = -1;

};
