#include <engine/core.h>
#include <engine/render.h>

Engine::Render::VertexBuffers buffers;

float vertices[] = {
    -0.5f, -0.5f, 0.0f, // left
    0.5f, -0.5f, 0.0f,  // right
    0.0f, 0.5f, 0.0f    // top
};

std::string vertex =
    "#version 330 core\n"
    "layout (location = 0) in vec3 vertexPosition;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);\n"
    "}\0";

std::string fragment =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";

void Engine::Core::Application::Start()
{
    unsigned int shader = Engine::Render::GlobalRenderer->CompileShader(vertex, fragment);              // compile a shader program using the sources
    buffers = Engine::Render::GlobalRenderer->GenerateBuffers(vertices, sizeof(vertices)); // generate buffers
}

void Engine::Core::Application::Update()
{
    Engine::Render::GlobalRenderer->Draw(buffers); // draw the vertices
}