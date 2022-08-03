#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>
#include <iostream>

Engine::Render::VertexBuffers buffers;

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // right
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,    // top
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // left
    0.5f, -0.5f, 0.0f, 1.0f,0.0f, 0.0f,  // right
    0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // top
    100.0f // end
};

class SampleComponent : public Engine::ECS::Component
{
public:
    void Start()
    {
        std::cout << "Start\n";
    }

    void Update()
    {
        std::cout << "SampleComponent\n";
    }
};

class SampleObject : public Engine::ECS::GameObject
{
public:
    void Start()
    {
        std::cout << "Start\n";
        AddComponent(new SampleComponent);
    }

    void Update()
    {
        Engine::ECS::GameObject::Update(); // update components

        std::cout << "SampleObject\n";
    }
};

void Engine::Core::Application::Start()
{
    buffers = Engine::Render::GlobalRenderer->GenerateBuffers(vertices); // generate buffers

    Engine::ECS::GlobalGameObjectManager->Add(new SampleObject);
}

void Engine::Core::Application::Update()
{
    Engine::Render::GlobalRenderer->Draw(buffers); // draw the vertices
}