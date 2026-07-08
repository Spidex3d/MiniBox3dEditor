#pragma once
#include <string>
#include <boXM\boXM.h>

enum class SceneObjectType
{
    Mesh,
    Camera,
    Light,
    Empty
};

struct SceneObject
{
    int id = 0;
    std::string name = "Object";

    SceneObjectType type = SceneObjectType::Mesh;

    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    vec3 rotation = vec3(0.0f, 0.0f, 0.0f);
    vec3 scale = vec3(1.0f, 1.0f, 1.0f);

    bool selected = false;
    bool expanded = false;
};
