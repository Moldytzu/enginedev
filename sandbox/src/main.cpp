#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>
#include <iostream>
#include <time.h>

class PlaneRenderer : public Engine::ECS::Component
{
private:
    Engine::Render::VertexBuffers buffers;

public:
    std::vector<Engine::Render::Vertex> vertices =
        {
            // left triangle
            Engine::Render::Vertex(-1, 0, 1, 1, 1, 1, 0, 1),  // left-top
            Engine::Render::Vertex(-1, 0, -1, 1, 1, 1, 0, 0), // left-bottom
            Engine::Render::Vertex(1, 0, -1, 1, 1, 1, 1, 0),  // right-bottom

            // right triangle
            Engine::Render::Vertex(1, 0, 1, 1, 1, 1, 1, 1),  // right-top corner
            Engine::Render::Vertex(1, 0, -1, 1, 1, 1, 1, 0), // right-bottom
            Engine::Render::Vertex(-1, 0, 1, 1, 1, 1, 0, 1), // left-top

        };

    void Start()
    {
        // initial public variables states
        Public.SetString("texturePath", "");   // path to the texture
        Public.SetBool("flushTexture", false); // flag to use when flushing texture and loading a new one
    }

    void Update()
    {
        // generate buffers if flush is requested
        if (Public.GetBool("flushTexture"))
        {
            buffers = Engine::Render::GlobalRenderer->GenerateBuffers(vertices, Engine::Render::GlobalRenderer->DefaultShader, Engine::Render::GlobalRenderer->LoadTexture(Public.GetString("texturePath"))); // generate new buffers
            Public.SetBool("flushTexture", false);                                                                                                                                                            // reset flag state
        }
        else
        {
            float speed = 10 * Engine::Render::GlobalRenderer->DeltaTime;
            Parent->Transform.Rotate(1 * speed, glm::vec3(Engine::Core::Random::Float(0, 1) * speed, Engine::Core::Random::Float(0, 1) * speed, Engine::Core::Random::Float(0, 1) * speed)); // scale it by 2 and rotate it randomly
            Engine::Render::GlobalRenderer->Draw(buffers, Parent->Transform);                                                                                                                // draw the vertices
        }
    }

    std::string FriendlyName()
    {
        return "PlaneRenderer";
    }
};

class MyPlane : public Engine::ECS::GameObject
{
public:
    void Start()
    {
        AddComponent(new PlaneRenderer)->Public.SetString("texturePath", "bricks.jpg")->SetBool("flushTexture", true); // add the renderer and then set the texture path and flush the texture
    }

    void Update()
    {
        Engine::ECS::GameObject::Update(); // update components
    }

    std::string FriendlyName()
    {
        return "MyPlane";
    }
};

void Engine::Core::Application::Start()
{
    Engine::ECS::GlobalGameObjectManager->Add(new MyPlane);
    Engine::Render::GlobalRenderer->CameraTransform.Translate(glm::vec3(0, 1, -2)); // translate the camera up and back
}

void Engine::Core::Application::Update()
{
}