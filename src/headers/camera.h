#pragma once

struct SCamera
{
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    const float MovementSpeed = 0.5f;
    float MouseSensitivity = 0.25f;
};

void InitCamera(SCamera &in, float y, float p)
{
    in.Front = glm::vec3(0.0f, 0.0f, -1.0f);
    in.Position = glm::vec3(0.0f, 0.0f, 0.0f);
    in.Up = glm::vec3(0.0f, 1.0f, 0.0f);
    in.WorldUp = in.Up;
    in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));

    in.Yaw = y;
    in.Pitch = p;
}
