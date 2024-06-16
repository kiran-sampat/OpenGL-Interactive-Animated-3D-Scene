#pragma once

/* ---- Standard Library ---- */
#include <cstdio>

/* ---- GLM Includes ---- */
#ifdef _WIN32
#include <glm/glm/glm.hpp>
#endif

#ifdef __unix
#include <glm/glm.hpp>
#endif

/* ---- Header Files ---- */
#include "camera.h"

void MoveCamera(SCamera &in, SCamera::Camera_Movement direction)
{
    if (direction == in.FORWARD)
        in.Position += in.Front * in.MovementSpeed;
    if (direction == in.BACKWARD)
        in.Position -= in.Front * in.MovementSpeed;
    if (direction == in.LEFT)
        in.Position -= in.Right * in.MovementSpeed;
    if (direction == in.RIGHT)
        in.Position += in.Right * in.MovementSpeed;
}

void OrientCamera(SCamera &in, float xoffset, float yoffset)
{
    in.Yaw += xoffset * in.MouseSensitivity;
    in.Pitch += yoffset * in.MouseSensitivity;

    if (in.Pitch > 89.0f)
        in.Pitch = 89.0f;
    if (in.Pitch < -89.0f)
        in.Pitch = -89.0f;
        
    glm::vec3 pos;

    float pitch = glm::radians(in.Pitch);
    float yaw = glm::radians(in.Yaw);

    pos.x = glm::cos(yaw) * glm::cos(pitch);
    pos.y = glm::sin(pitch);
    pos.z = glm::sin(yaw) * glm::cos(pitch);

    in.Front = glm::normalize(pos);
    in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
    in.Up = glm::normalize(glm::cross(in.Right, in.Front));
}

