#pragma once

/* ---- OpenGL Headers ---- */
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int w, int h)
{
    // Specify the viewport of OpenGL in the Window
    glViewport(0, 0, w, h);
}

GLFWwindow *Create_Window(int w, int h, const char *title)
{
    // Step 1 - Initialise GLFW
    glfwInit();

    // Step 2 - Specify OpenGL version that is being used, in this case OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile, meaning we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Multi Sampling
    glfwWindowHint(GLFW_SAMPLES, 16);

    // Step 3 - Create a window
    GLFWwindow *window = glfwCreateWindow(w, h, title, NULL, NULL);

    // Step 4 - Specify drawing should be to the window
    glfwMakeContextCurrent(window);

    // Step 5 - Specify a window resize function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}
