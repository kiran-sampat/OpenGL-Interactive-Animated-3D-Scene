/* ---- Standard Library ---- */
#include <cstdio>

/* ---- OpenGL Headers ---- */
#define GLFW_INCLUDE_NONE // Allows for opengl includes in any order
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* ---- GLM Includes ---- */
#ifdef _WIN32
    #include <glm/glm/glm.hpp>
    #include <glm/glm/gtc/matrix_transform.hpp>
    #include <glm/glm/gtc/type_ptr.hpp>
#endif

#ifdef __unix
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
#endif

/* ---- Header Files ---- */
#include "headers/window.h"
#include "headers/shader.h"
#include "headers/texture.h"
#include "headers/camera.h"
#include "headers/ModelViewerCamera.h"
#include "headers/FlyThroughCamera.h"
#include "headers/parser.h"

/* ---- Function Prototypes ---- */
void processKeyboard(GLFWwindow *window);
void processMouse(GLFWwindow *window, double x, double y);

/* ---- Definitions ---- */
#define PIXEL_W 1280
#define PIXEL_H 720

/* ---- Global Vars and Constants ---- */
// Declare Light Variables
// Set the light direction vars
glm::vec3 lightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 lightDirection_scene = glm::vec3(0.0f, -1.0f, 0.0f);
// Set the light position vars
glm::vec3 lightPos = glm::vec3(0.f, 4.f, 0.f);
glm::vec3 lightPos_scene = glm::vec3(0.f, 8.f, 0.f);

// Declare Cameras
SCamera Camera_FT;  // Fly Through Camera
SCamera Camera_MV;  // Model Viewer Camera

bool is_fly_through = true;

float cam_dist = 0.f;

float y_rotation_angle = 0.0f;

/* Main Function */
int main(int argc, char *argv[])
{
    // Create pairs from the parsed OBJ data
    std::pair<float *, unsigned int> pair_island   = parse_OBJ("models/island.obj");
    std::pair<float *, unsigned int> pair_stadium  = parse_OBJ("models/stadium.obj");
    std::pair<float *, unsigned int> pair_podium   = parse_OBJ("models/podium.obj");
    std::pair<float *, unsigned int> pair_statue_1 = parse_OBJ("models/metalgreymon.obj");
    std::pair<float *, unsigned int> pair_statue_2 = parse_OBJ("models/weregarurumon.obj");
    std::pair<float *, unsigned int> pair_agumon   = parse_OBJ("models/agumon.obj");
    std::pair<float *, unsigned int> pair_gabumon  = parse_OBJ("models/gabumon.obj");
    std::pair<float *, unsigned int> pair_tree     = parse_OBJ("models/tree.obj");

    // Declare Vertex Arrays
    float *vertices_island   = pair_island.first;
    float *vertices_stadium  = pair_stadium.first;
    float *vertices_podium   = pair_podium.first;
    float *vertices_statue_1 = pair_statue_1.first;
    float *vertices_statue_2 = pair_statue_2.first;
    float *vertices_agumon   = pair_agumon.first;
    float *vertices_gabumon  = pair_gabumon.first;
    float *vertices_tree     = pair_tree.first;

    // Create GLFW Window
    GLFWwindow *window = Create_Window(PIXEL_W, PIXEL_H, "Computer Graphics Assessment 3");

    // Setup Mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, processMouse);

    // Initialize GLAD Loader to Configure OpenGL
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Load GLSL Vertex and Fragment Shaders
    unsigned int shaderProgram = LoadShader("shaders/vertex.vert", "shaders/fragment.frag");

    // Initialize Fly Through Camera
    InitCamera(Camera_FT, 45, -15);
    OrientCamera(Camera_FT, 0, 0);

    // Initialize Model Viewer Camera
    InitCamera(Camera_MV, 0, 0);
    cam_dist = 15.0f;
    float cam_x_offset = 360.f;
    float cam_y_offset = -45.f;
    MoveAndOrientCamera(Camera_MV, glm::vec3(0, 0, 0), cam_dist, cam_x_offset, cam_y_offset);

    // Setup All Textures - Some textures have Mipmaps applied
    GLuint texture_island   = setup_mipmaps("textures/island.bmp");
    GLuint texture_stadium  = setup_texture("textures/stadium.bmp");
    GLuint texture_podium   = setup_texture("textures/podium.bmp");
    GLuint texture_statue_1 = setup_mipmaps("textures/metalgreymon.bmp");
    GLuint texture_statue_2 = setup_mipmaps("textures/weregarurumon.bmp");
    GLuint texture_agumon   = setup_texture("textures/agumon.bmp");
    GLuint texture_gabumon  = setup_texture("textures/gabumon.bmp");
    GLuint texture_tree     = setup_texture("textures/tree.bmp");

    // Create reference container for the VAO/VBO and Generate with 8 objects
    unsigned int VAO[8];
    glGenVertexArrays(8, VAO);
    unsigned int VBO[8];
    glGenBuffers(8, VBO);

    /* Object 0 - Island */
    // Bind the VAO to make it the current Vertex Array Object
    glBindVertexArray(VAO[0]);
    // Bind the VBO specifying it is a GL_ARRAY_BUFFER Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // Copy the specified vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * pair_island.second * 8, vertices_island, GL_STATIC_DRAW);
    // Set the Vertex Attribute Pointers so that OpenGL knows how to read the VBO
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);  // v position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // v texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);  // v normal

    /* Object 1 - Stadium */
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * pair_stadium.second * 8, vertices_stadium, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);  // v position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // v texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);  // v normal

    /* Object 2 - Podium */
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * pair_podium.second * 8, vertices_podium, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);  // v position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // v texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);  // v normal

    /* Object 3 - Statue 1 - MetalGreymon */
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * pair_statue_1.second * 8, vertices_statue_1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);  // v position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // v texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);  // v normal

    /* Object 4 - Statue 2 - WereGarurumon */
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * pair_statue_2.second * 8, vertices_statue_2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);  // v position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // v texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);  // v normal

    /* Object 5 - Agumon */
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * pair_agumon.second * 8, vertices_agumon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);  // v position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // v texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);  // v normal

    /* Object 6 - Gabumon */
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * pair_gabumon.second * 8, vertices_gabumon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);  // v position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // v texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);  // v normal

    /* Object 7 - Tree */
    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, (long) sizeof(float) * pair_tree.second * 8, vertices_tree, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);  // v position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // v texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);  // v normal

    // Bind both the VBO and VAO to 0
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Tell OpenGL which Shader Program to use
    glUseProgram(shaderProgram);

    // Main Render Loop
    while (!glfwWindowShouldClose(window))
    {
        processKeyboard(window);

        // Specify the background color
        glClearColor(0.05f, 0.15f, 0.5f, 1.f);

        // Clear the back buffer and assign the new color to it
        // Clear the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Controls the interpolation of polygons for Rasterization
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Transfer uniform values of Light 1 to the shaders
        glUniform3f(glGetUniformLocation(shaderProgram, "light_1.lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "light_1.lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "light_1.lightColor"), 1.0f, 1.0f, 1.0f);

        // Transfer uniform values of Light 2 to the shaders
        glUniform3f(glGetUniformLocation(shaderProgram, "light_2.lightDirection"), lightDirection_scene.x, lightDirection_scene.y, lightDirection_scene.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "light_2.lightPos"), lightPos_scene.x, lightPos_scene.y, lightPos_scene.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "light_2.lightColor"), 1.0f, 1.0f, 1.0f);

        // Transfer uniform values of the camera position to the shaders, depending on the camera type
        if (is_fly_through)
            glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), Camera_FT.Position.x, Camera_FT.Position.y, Camera_FT.Position.z);
        else
            glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), Camera_MV.Position.x, Camera_MV.Position.y, Camera_MV.Position.z);

        // Setup and Copy the View Matrix
        glm::mat4 view = glm::mat4(1.f);
        view = glm::translate(view, -glm::vec3(0.f, 0.f, 0.f));

        if (is_fly_through)
            view = glm::lookAt(Camera_FT.Position, Camera_FT.Position + Camera_FT.Front, Camera_FT.Up);
        else
            view = glm::lookAt(Camera_MV.Position, Camera_MV.Position + Camera_MV.Front, Camera_MV.Up);

        int v_loc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(v_loc, 1, GL_FALSE, glm::value_ptr(view));

        // Setup and Copy the Projection Matrix
        glm::mat4 projection = glm::mat4(1.f);
        projection = glm::perspective(glm::radians(45.f), (float) 800 / (float) 600, .1f, 200.f);
        int p_loc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(p_loc, 1, GL_FALSE, glm::value_ptr(projection));

        // Setup and Copy all the Model Matrices
        // Island - Model 0
        glm::mat4 model_island = glm::mat4(1.f);
        // Declare Transformations
        model_island = glm::translate(model_island, glm::vec3(0.0f, 0.0f, 0.0f));
        model_island = glm::rotate(model_island, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_island = glm::scale(model_island, glm::vec3(0.225f, 0.225f, 0.225f));
        // Transfer uniform value of the specified model matrix to the shaders
        int m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_island));
        // Set the model texture
        glBindTexture(GL_TEXTURE_2D, texture_island);
        glUseProgram(shaderProgram);
        // Set and Draw Triangles
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_island.second);

        // Stadium - Model 1
        glm::mat4 model_stadium = glm::mat4(1.f);
        model_stadium = glm::translate(model_stadium, glm::vec3(0.0f, 0.0f, 0.0f));
        model_stadium = glm::rotate(model_stadium, (float) glfwGetTime() / 4, glm::vec3(0.0f, 1.0f, 0.0f));
        model_stadium = glm::scale(model_stadium, glm::vec3(0.15f, 0.15f, 0.15f));
        m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_stadium));
        glBindTexture(GL_TEXTURE_2D, texture_stadium);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_stadium.second);

        // Podium - Model 2
        glm::mat4 model_podium = glm::mat4(1.f);
        model_podium = glm::translate(model_podium, glm::vec3(0.0f, 0.0f, 0.0f));
        model_podium = glm::rotate(model_podium, glm::radians(45.f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_podium = glm::scale(model_podium, glm::vec3(0.4f, 0.4f, 0.4f));
        m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_podium));
        glBindTexture(GL_TEXTURE_2D, texture_podium);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_podium.second);

        // Statue 1 - Model 3
        glm::mat4 model_statue_1 = glm::mat4(1.f);
        model_statue_1 = glm::translate(model_statue_1, glm::vec3(-0.2f, 0.5f, 0.1f));
        model_statue_1 = glm::rotate(model_statue_1, glm::radians(210.f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_statue_1 = glm::scale(model_statue_1, glm::vec3(0.55f, 0.55f, 0.55f));
        m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_statue_1));
        glBindTexture(GL_TEXTURE_2D, texture_statue_1);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_statue_1.second);

        // Statue 2 - Model 4
        glm::mat4 model_statue_2 = glm::mat4(1.f);
        model_statue_2 = glm::translate(model_statue_2, glm::vec3(0.1f, 0.5f, -0.2f));
        model_statue_2 = glm::rotate(model_statue_2, glm::radians(230.f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_statue_2 = glm::scale(model_statue_2, glm::vec3(0.55f, 0.55f, 0.55f));
        m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_statue_2));
        glBindTexture(GL_TEXTURE_2D, texture_statue_2);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[4]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_statue_2.second);

        // Agumon - Model 5
        glm::mat4 model_agumon = glm::mat4(1.f);
        model_agumon = glm::translate(model_agumon, glm::vec3(0.0f, 0.0f, -1.25f));
        model_agumon = glm::rotate(model_agumon, glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_agumon = glm::scale(model_agumon, glm::vec3(0.6f, 0.6f, 0.6f));
        m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_agumon));
        glBindTexture(GL_TEXTURE_2D, texture_agumon);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[5]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_agumon.second);

        // Gabumon - Model 6
        glm::mat4 model_gabumon = glm::mat4(1.f);
        model_gabumon = glm::translate(model_gabumon, glm::vec3(-1.25f, 0.0f, 0.0f));
        model_gabumon = glm::rotate(model_gabumon, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_gabumon = glm::scale(model_gabumon, glm::vec3(0.6f, 0.6f, 0.6f));
        m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_gabumon));
        glBindTexture(GL_TEXTURE_2D, texture_gabumon);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[6]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_gabumon.second);

        // Tree 1 - Model 7
        glm::mat4 model_tree_1 = glm::mat4(1.f);
        model_tree_1 = glm::translate(model_tree_1, glm::vec3(1.5f, 0.0f, -1.0f));
        model_tree_1 = glm::rotate(model_tree_1, glm::radians(y_rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model_tree_1 = glm::scale(model_tree_1, glm::vec3(0.5f, 0.5f, 0.5f));
        m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_tree_1));
        glBindTexture(GL_TEXTURE_2D, texture_tree);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[7]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_tree.second);

        // Tree 2 - Model 7
        glm::mat4 model_tree_2 = glm::mat4(1.f);
        model_tree_2 = glm::translate(model_tree_2, glm::vec3(-1.0f, 0.0f, 1.5f));
        model_tree_2 = glm::rotate(model_tree_2, glm::radians(y_rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model_tree_2 = glm::scale(model_tree_2, glm::vec3(0.5f, 0.5f, 0.5f));
        m_loc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(model_tree_2));
        glBindTexture(GL_TEXTURE_2D, texture_tree);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[7]);
        glDrawArrays(GL_TRIANGLES, 0, (int) pair_tree.second);

        glBindVertexArray(0);

        // Swap buffers so the image gets updated with each frame
        glfwSwapBuffers(window);
    }

    // Delete all the objects that were created
    glDeleteVertexArrays(8, VAO);
    glDeleteBuffers(8, VBO);
    glDeleteProgram(shaderProgram);

    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();

    return 0;
}

/* Function to Process Keyboard Input */
void processKeyboard(GLFWwindow *window)
{
    // Processes all the GLFW events
    glfwPollEvents();

    // Close window when escape is hit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)        
        glfwSetWindowShouldClose(window, true);

    // Left Angle Bracket
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
        y_rotation_angle += 5.f;
    // Right Angle Bracket
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
        y_rotation_angle -= 5.f;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        is_fly_through = false;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        is_fly_through = true;
    }

    if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))
    {
        if (is_fly_through) {
            lightDirection = Camera_FT.Front;
            lightPos = Camera_FT.Position;
        }
        else {
            lightDirection = Camera_MV.Front;
            lightPos = Camera_MV.Position;
        }
    }

    float cam_x_offset = 0.f;
    float cam_y_offset = 0.f;
    bool cam_changed = false;

    if (is_fly_through)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            MoveCamera(Camera_FT, SCamera::FORWARD);
            cam_changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            MoveCamera(Camera_FT, SCamera::LEFT);
            cam_changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            MoveCamera(Camera_FT, SCamera::BACKWARD);
            cam_changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            MoveCamera(Camera_FT, SCamera::RIGHT);
            cam_changed = true;
        }
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            cam_x_offset = 0.f;
            cam_y_offset = -1.f;
            cam_changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            cam_x_offset = 0.f;
            cam_y_offset = 1.f;
            cam_changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            cam_x_offset = -1.f;
            cam_y_offset = 0.f;
            cam_changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            cam_x_offset = 1.f;
            cam_y_offset = 0.f;
            cam_changed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            cam_dist -= 0.2f * Camera_MV.MovementSpeed;
            cam_changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            cam_dist += 0.2f * Camera_MV.MovementSpeed;
            cam_changed = true;
        }
    }

    if (cam_changed)
    {
        if (!is_fly_through) {
            MoveAndOrientCamera(Camera_MV, glm::vec3(0, 0, 0), cam_dist, cam_x_offset, cam_y_offset);
        }

        //printf("DIST: %f, X-OFFSET: %f, Y-OFFSET: %f\n", cam_dist, cam_x_offset, cam_y_offset);
        //printf("[CAM POS]           X: %f \t| Y: %f \t| Z: %f \n", Camera.Position.x, Camera.Position.y, Camera.Position.z);
        //printf("[CAM FRONT]         X: %f \t| Y: %f \t| Z: %f \n", Camera.Front.x, Camera.Front.y, Camera.Front.z);
        //printf("[LIGHT POS]         X: %f \t| Y: %f \t| Z: %f \n", lightPos.x, lightPos.y, lightPos.z);
        //printf("[LIGHT DIRECTION]   X: %f \t| Y: %f \t| Z: %f \n", lightDirection.x, lightDirection.y, lightDirection.z);
    }
}

bool firstMouse = true;
double prevMouseX;
double prevMouseY;

/* Function to Process Mouse Input */
void processMouse(GLFWwindow *window, double x, double y)
{
    if (firstMouse)
    {
        prevMouseX = x;
        prevMouseY = y;
        firstMouse = false;
    }

    double dX = x - prevMouseX;
    double dY = prevMouseY - y;

    prevMouseX = x;
    prevMouseY = y;

    if (is_fly_through) {
        OrientCamera(Camera_FT, (float) dX, (float) dY);
    }
}

