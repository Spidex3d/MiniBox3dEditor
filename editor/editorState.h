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
    Rotate,
    Scale,
    Extrude
};

struct EditorState
{
    EditorMode mode = EditorMode::ObjectMode;
    EditSelectMode selectMode = EditSelectMode::Vertex;
    EditorTool activeTool = EditorTool::None;
};
