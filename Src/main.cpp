#include "Errors.h"
#include <cstdlib>
#define GLM_FORCE_CTOR_INIT
#define GLEW_STATIC
#include "glew.h"
#include "glfw3.h"

#include "Game.h"
#include "ResourceManager.h"

#include<cmath>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);
int main(int argc, char *argv[])
{
	Error status = ERR_NO_ERR;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

    glfwSetKeyCallback(window, key_callback);

    // OpenGL configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize game
    CHECK(Breakout.init());

    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Start Game within Menu State
    Breakout.state = GAME_ACTIVE;

    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = std::abs(currentFrame - lastFrame);
        lastFrame = currentFrame;
        glfwPollEvents();

        //deltaTime = 0.001f;
        // Manage user input
        Breakout.processInput(deltaTime);

        // Update Game state
        Breakout.update(deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.render();

        glfwSwapBuffers(window);
    }

    // Delete all resources as loaded using the resource manager
//    ResourceManager::clear();
	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Breakout.keys[key] = GL_FALSE;
    }
}
