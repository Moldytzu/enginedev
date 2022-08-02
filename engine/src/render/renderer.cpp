#include <engine/render.h>
#include <engine/vendor/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow *window;

void resize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // reset the viewport
}

void Engine::Render::Renderer::Init()
{
    std::cout << "Starting up the renderer\n";

    glfwInit();                                    // initialise glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // hint that we want an opengl 3.3 core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Main Window", NULL, NULL); // create the main window
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window); // select the context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // load glad
        std::cout << "Failed to initialize GLAD" << std::endl;

    glfwSetFramebufferSizeCallback(window, resize); // resize callback

    glEnable(GL_DEPTH);         // depth checking
    glViewport(0, 0, 640, 480); // set the viewport
}

void Engine::Render::Renderer::StartFrame()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // grayish colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window and the depth buffer
}

void Engine::Render::Renderer::EndFrame()
{
    glfwSwapBuffers(window); // swap the buffers
    glfwPollEvents();        // poll for the events
}

bool Engine::Render::Renderer::Open()
{
    return !glfwWindowShouldClose(window); // returns true when the window shouldn't close / when it's open
}

Engine::Render::Renderer::~Renderer()
{
    std::cout << "Destroying the renderer\n";
    glfwTerminate();
}