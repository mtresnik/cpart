#include <iostream>
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "linmath.h"
#include "particle_engine.h"

#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "converter.h"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        ParticleEngine* particleEngine = ParticleEngine::getInstance();
        particleEngine->onClick(xPos, yPos);
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        ParticleEngine* particleEngine = ParticleEngine::getInstance();
        particleEngine->onClick(xPos, yPos);
    }
}

int main(void)
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(1280, 720, "Sand Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    // Initialize mouse move and mouse click
    // Mouse move should update the position of a circle on the screen. Possibly a transparent texture.

    Converter* converter = Converter::getInstance();
    converter->init(window);

    ParticleEngine* particleEngine = ParticleEngine::getInstance();
    particleEngine->init();
    while (!glfwWindowShouldClose(window))
    {
        particleEngine->update(0.05);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        particleEngine->render();

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    particleEngine->destroy();
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
