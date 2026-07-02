#include "Camera.h"



Camera::Camera(vec3 position, vec3 up, float yaw, float pitch)
    : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}



mat4 Camera::GetViewMatrix()
{
    return lookAt(Position, Position + Front, Up);
}

mat4 Camera::GetProjectionMatrix(float aspectRatio)
{
    return perspective(Zoom, aspectRatio, 0.1f, 100.0f);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    vec3 front;
    front.x = cos(radians(Yaw)) * cos(radians(Pitch));
    front.y = sin(radians(Pitch));
    front.z = sin(radians(Yaw)) * cos(radians(Pitch));
    Front = normalize(front);
    Right = normalize(cross(Front, WorldUp));
    Up = normalize(cross(Right, Front));
}
void Camera::ProcessOrbit(float xoffset, float yoffset)
{
    xoffset *= OrbitSensitivity;
    yoffset *= OrbitSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;

    if (Pitch < -89.0f)
        Pitch = -89.0f;

    UpdateOrbitCamera();
}



void Camera::ProcessOrbitZoom(float yoffset)
{
    Distance -= yoffset * ZoomSensitivity;

    if (Distance < 1.0f)
        Distance = 1.0f;

    if (Distance > 50.0f)
        Distance = 50.0f;

    UpdateOrbitCamera();
}

void Camera::ProcessOrbitPan(float xoffset, float yoffset)
{
    float panAmount = Distance * PanSensitivity;

    vec3 panRight = Right * (-xoffset * panAmount);
    vec3 panUp = Up * (yoffset * panAmount);

    Target += panRight;
    Target += panUp;

    UpdateOrbitCamera();
}

void Camera::UpdateOrbitCamera()
{
    float yawRad = radians(Yaw);
    float pitchRad = radians(Pitch);

    vec3 direction;

    direction.x = cos(yawRad) * cos(pitchRad);
    direction.y = sin(pitchRad);
    direction.z = sin(yawRad) * cos(pitchRad);

    direction = normalize(direction);

    // Camera sits behind the target, looking toward it.
    Position = Target - direction * Distance;

    Front = normalize(Target - Position);
    Right = normalize(cross(Front, WorldUp));
    Up = normalize(cross(Right, Front));
}