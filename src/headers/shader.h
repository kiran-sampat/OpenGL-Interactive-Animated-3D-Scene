#pragma once

/* ---- Standard Library ---- */
#include <iostream>

/* ---- OpenGL Headers ---- */
#include <glad/glad.h>

/* ---- Header Files ---- */
#include "util.h"

unsigned int LoadShader(const char *vertexShaderFile, const char *fragmentShaderFile)
{
    int success;
    char infoLog[512];

    // Create Vertex Shader Object and get its reference
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Read Vertex Shader file and store in Vertex Shader source
    char *vertexShaderSource = read_file(vertexShaderFile);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // Error checking for Vertex Shader
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Create Fragment Shader Object and get its reference
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Read Fragment Shader file and store in Fragment Shader source
    char *fragmentShaderSource = read_file(fragmentShaderFile);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile the Fragment Shader into machine code
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    // Error checking for Fragment Shader
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Create Shader Program Object and get its reference
    unsigned int shaderProgram = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(shaderProgram);

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Free memory allocated during read_file to the Vertex and Fragment Shader sources
    free(vertexShaderSource);
    free(fragmentShaderSource);

    return shaderProgram;
}

