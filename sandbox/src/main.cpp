#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>
#include <iostream>

Engine::Render::VertexBuffers buffers;

std::vector<Engine::Render::Vertex> vertices =
    {
        Engine::Render::Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Engine::Render::Vertex(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Engine::Render::Vertex(-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        Engine::Render::Vertex(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Engine::Render::Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        Engine::Render::Vertex(-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
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
        //std::cout << "SampleComponent\n";
    }

    std::string FriendlyName()
    {
        return "SampleComponent";
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

        // std::cout << "SampleObject\n";
    }

    std::string FriendlyName()
    {
        return "SampleObject";
    }
};

void Engine::Core::Application::Start()
{
    buffers = Engine::Render::GlobalRenderer->GenerateBuffers(vertices, Engine::Render::GlobalRenderer->DefaultShader, Engine::Render::GlobalRenderer->LoadTexture("bricks.jpg")); // generate buffers

    Engine::ECS::GlobalGameObjectManager->Add(new SampleObject);
    Engine::ECS::GlobalGameObjectManager->Add(new SampleObject);
    Engine::ECS::GlobalGameObjectManager->Add(new SampleObject);
    Engine::ECS::GlobalGameObjectManager->Add(new SampleObject, "Abc");
    Engine::ECS::GlobalGameObjectManager->Add(new SampleObject, "Abc");
    Engine::ECS::GlobalGameObjectManager->Add(new SampleObject, "Abc");
 
    Engine::Render::GlobalRenderer->CameraTransform.Translate(glm::vec3(0, 0, -10)); // translate the camera back
}

void Engine::Core::Application::Update()
{
    Engine::Render::Transform transform;                      // transform of the vertices
    Engine::Render::GlobalRenderer->Draw(buffers, transform); // draw the vertices
}