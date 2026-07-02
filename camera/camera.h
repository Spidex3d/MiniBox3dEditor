#pragma once
#include <boXM\boXM.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP
};

const float YAW = 90.0f; // was -90.0f, changed to 90.0f to face the positive x-axis
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:

    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    vec3 Target = vec3(0.0f, 0.0f, 5.0f);

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;


    float Distance = 6.0f;
    float OrbitSensitivity = 0.25f;
    float ZoomSensitivity = 0.5f;



    Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    mat4 GetViewMatrix();
    mat4 GetProjectionMatrix(float aspectRatio);

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    void ProcessOrbit(float xoffset, float yoffset);
    void ProcessOrbitZoom(float yoffset);
    void UpdateOrbitCamera();

private:
    void updateCameraVectors();
};

