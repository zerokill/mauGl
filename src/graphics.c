#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "dependencies/include/GL/glew.h"
#include "dependencies/include/GLFW/glfw3.h"
#include "shader.h"
#include "util.h"
#include "graphics.h"

// Global variables (replacing soon with State struct)
int totalFrames = 0;

int simpleShader(GLFWwindow* window, int width, int height)
{
    unsigned int phongShader = createShader("src/shader/baseVertex.glsl", "src/shader/fractal.glsl");
    float dt = 0.000001f;
    float lastFrameTime = (float)glfwGetTime();
    char title[100] = "";
    vec3_t VEC_CLEAR = {0.0f, 0.0f, 0.0f};
    scale_pos_t scale_pos;
    scale_pos.scale = VEC_CLEAR;
    scale_pos.position = VEC_CLEAR;

    // Vertices coordinates
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * (float)(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f, // Lower left corner
         0.5f, -0.5f * (float)(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f, // Lower right corner
         0.0f,  0.5f * (float)(sqrt(3)) * 2 / 3, 0.0f,  1.0f, 0.6f,  0.32f, // Upper corner
        -0.25f, 0.5f * (float)(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f, // Inner left
         0.25f, 0.5f * (float)(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f, // Inner right
         0.0f, -0.5f * (float)(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.2f   // Inner down
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Upper triangle
        5, 4, 1 // Lower right triangle
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        processInput(window, &scale_pos);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(phongShader);

        glUniform3fv(glGetUniformLocation(phongShader, "uScale"), 1, &scale_pos.scale.x);
        glUniform3fv(glGetUniformLocation(phongShader, "uPosition"), 1, &scale_pos.position.x);
        glUniform2f(glGetUniformLocation(phongShader, "iResolution"), width, height);
        glUniform1f(glGetUniformLocation(phongShader, "iTime"), glfwGetTime());


		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

        if (totalFrames % 60 == 0) {
            sprintf(title, "FPS : %-4.0f scale=%.2f", 1.0 / dt, scale_pos.scale.x);
            glfwSetWindowTitle(window, title);
        }

        // Timing
        dt = (float)glfwGetTime() - lastFrameTime;
        while (dt < 1.0f / TARGET_FPS) {
            dt = (float)glfwGetTime() - lastFrameTime;
        }
        lastFrameTime = (float)glfwGetTime();
        totalFrames++;


    }

    // Delete all the objects we've created
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(phongShader);

    return 0;

}

int shadertoy(GLFWwindow* window, int width, int height) {
    unsigned int phongShader = createShader("src/shader/baseVertex.glsl", "src/shader/base.glsl");

    float dt = 0.000001f;
    float lastFrameTime = (float)glfwGetTime();

    char title[100] = "";

    srand(time(NULL));

    vec3_t VEC_INIT = {1.0f, 1.0f, 1.0f};
    vec3_t VEC_CLEAR = {0.0f, 0.0f, 0.0f};
    scale_pos_t scale_pos;
    scale_pos.scale = VEC_INIT;
    scale_pos.position = VEC_CLEAR;

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, // Bottom-left vertex
        1.0f, -1.0f, 0.0f, // Bottom-right vertex
        -1.0f, 1.0f, 0.0f, // Top-left vertex
        1.0f, 1.0f, 0.0f // Top-right vertex
//        -0.5f, -0.5f, 0.0f, // left
//         0.5f, -0.5f, 0.0f, // right
//         0.0f,  0.5f, 0.0f  // top
    };

    GLuint quadVBO, quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        processInput(window, &scale_pos);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        GLint depthFuncValue;
        glGetIntegerv(GL_DEPTH_FUNC, &depthFuncValue);
        glClearDepth(depthFuncValue == GL_LESS ? 1.0f : 0.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Render here */

        glUseProgram(phongShader);

        glUniform3fv(glGetUniformLocation(phongShader, "uScale"), 1, &scale_pos.scale.x);
        glUniform3fv(glGetUniformLocation(phongShader, "uPosition"), 1, &scale_pos.position.x);
        glUniform2f(glGetUniformLocation(phongShader, "iResolution"), width, height);
        glUniform1f(glGetUniformLocation(phongShader, "iTime"), glfwGetTime());

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        if (totalFrames % 60 == 0) {
            sprintf(title, "FPS : %-4.0f %s", 1.0 / dt, glGetString(GL_VERSION));
            glfwSetWindowTitle(window, title);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        /* Timing */
        dt = (float)glfwGetTime() - lastFrameTime;
        while (dt < 1.0f / TARGET_FPS) {
            dt = (float)glfwGetTime() - lastFrameTime;
        }
        lastFrameTime = (float)glfwGetTime();
        totalFrames++;

    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, scale_pos_t *scale_pos) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        scale_pos->scale.x += 0.01f;
        scale_pos->scale.y += 0.01f;
        scale_pos->scale.z = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        scale_pos->scale.x -= 0.01f;
        scale_pos->scale.y -= 0.01f;
        scale_pos->scale.z = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        scale_pos->scale.x = 0.3f;
        scale_pos->scale.y = 0.3f;
        scale_pos->scale.z = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        scale_pos->position.y += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        scale_pos->position.y -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        scale_pos->position.x -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        scale_pos->position.x += 0.01f;
    }
}
