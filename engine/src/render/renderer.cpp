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
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);               // grayish colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window and the depth buffer
}

void Engine::Render::Renderer::EndFrame()
{
    glfwSwapBuffers(window); // swap the buffers
    glfwPollEvents();        // poll for the events
}

unsigned int Engine::Render::Renderer::CompileShader(std::string vertex, std::string fragment)
{
    const char *vertexSrc = vertex.c_str(), *fragmentSrc = fragment.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER), fragmentShader = glCreateShader(GL_FRAGMENT_SHADER), shaderProgram = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Failed to compile vertex shader (" << infoLog << ")" << std::endl;
        return -1;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Failed to compile fragment shader (" << infoLog << ")" << std::endl;
        return -1;
    }

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Failed to link program (" << infoLog << ")" << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    shaders.push_back(shaderProgram);

    return shaderProgram;
}

Engine::Render::VertexBuffers Engine::Render::Renderer::GenerateBuffers(float *vertices, size_t size)
{
    Engine::Render::VertexBuffers buffers;
    glGenVertexArrays(1, &buffers.VAO);
    glGenBuffers(1, &buffers.VBO);
    glBindVertexArray(buffers.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return buffers;
}

void Engine::Render::Renderer::Draw(Engine::Render::VertexBuffers buffer, size_t vertices)
{
    buffer.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertices);
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