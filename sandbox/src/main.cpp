#include <engine/core.h>
#include <engine/render.h>

Engine::Render::VertexBuffers buffers;

float vertices[] = {
    -0.5f, -0.5f, 0.0f, // left
    0.5f, -0.5f, 0.0f,  // right
    0.0f, 0.5f, 0.0f    // top
};

void Engine::Core::Application::Start()
{
    buffers = Engine::Render::GlobalRenderer->GenerateBuffers(vertices, sizeof(vertices), Engine::Render::GlobalRenderer->DefaultShader); // generate buffers
}

void Engine::Core::Application::Update()
{
    Engine::Render::GlobalRenderer->Draw(buffers); // draw the vertices
}