#pragma once

/* ---- Standard Library ---- */
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector> // for std::vector
#include <utility> // for std::pair

/* ---- GLM Includes ---- */
#ifdef _WIN32
#include <glm/glm/vec2.hpp>
#include <glm/glm/vec3.hpp>
#endif

#ifdef __unix
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#endif

/* ---- Function Prototypes ---- */
std::pair<float *, unsigned int> parse_OBJ(const char *file_path);
void process_file_OBJ(const char *file_path);
void process_data_OBJ();
float *create_vertices();

