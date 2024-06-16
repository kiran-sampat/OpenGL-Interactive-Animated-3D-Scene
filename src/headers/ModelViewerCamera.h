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

void MoveAndOrientCamera(SCamera &in, glm::vec3 target, float distance, float xoffset, float yoffset)
{
    in.Yaw -= xoffset * in.MovementSpeed;
    in.Pitch -= yoffset * in.MovementSpeed;

    if (in.Pitch > 89.f)
       in.Pitch = 89.f;
    if (in.Pitch < -89.f)
       in.Pitch = -89.f;

    float pitch = glm::radians(in.Pitch);
    float yaw = glm::radians(in.Yaw);

    in.Position.x = glm::cos(yaw) * glm::cos(pitch) * distance;
    in.Position.y = glm::sin(pitch) * distance;
    in.Position.z = glm::sin(yaw) * glm::cos(pitch) * distance;

    in.Front = glm::normalize(target - in.Position);
    in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
    in.Up = glm::normalize(glm::cross(in.Right, in.Front));
}

