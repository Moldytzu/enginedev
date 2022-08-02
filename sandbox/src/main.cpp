#include <engine/core.h>
#include <engine/vendor/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void Engine::Core::Application::Start()
{
    glfwInit();                                    // initialise glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // hint that we want an opengl 3.3 core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Main Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, 640, 480);
}

void Engine::Core::Application::Update()
{
    std::cout << "Update\n";
}