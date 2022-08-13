#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>
#include <iostream>
#include <time.h>

class BaseRenderer : public Engine::ECS::Component
{
protected:
    Engine::Render::VertexBuffers buffers;
    std::vector<Engine::Render::Vertex> vertices;

public:
    virtual void Start()
    {
        // initial public variables states
        Public.SetString("texturePath", "");   // path to the texture
        Public.SetBool("flushTexture", false); // flag to use when flushing texture and loading a new one
    }

    virtual void Update()
    {
        // generate buffers if flush is requested
        if (Public.GetBool("flushTexture"))
        {
            buffers = Engine::Render::GlobalRenderer->GenerateBuffers(vertices, Engine::Render::GlobalRenderer->DefaultShader, Engine::Render::GlobalRenderer->LoadTexture(Public.GetString("texturePath"))); // generate new buffers
            Public.SetBool("flushTexture", false);                                                                                                                                                            // reset flag state
        }

        Engine::Render::GlobalRenderer->Draw(buffers, Parent->Transform); // draw the vertices
    }

    virtual std::string FriendlyName()
    {
        return "BaseRenderer";
    }
};

class PlaneRenderer : public BaseRenderer
{
public:
    void Start()
    {
        BaseRenderer::Start(); // initialise the base renderer class

        // vertices information
        vertices =
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
    }

    std::string FriendlyName()
    {
        return "PlaneRenderer";
    }
};

class CubeRenderer : public BaseRenderer
{
public:
    void Start()
    {
        BaseRenderer::Start(); // initialise the base renderer class

        // vertices information
        vertices =
            {
                // bottom face
                // left triangle
                Engine::Render::Vertex(-1, -1, 1, 1, 1, 1, 0, 1),  // left-top
                Engine::Render::Vertex(-1, -1, -1, 1, 1, 1, 0, 0), // left-bottom
                Engine::Render::Vertex(1, -1, -1, 1, 1, 1, 1, 0),  // right-bottom

                // right triangle
                Engine::Render::Vertex(1, -1, 1, 1, 1, 1, 1, 1),  // right-top corner
                Engine::Render::Vertex(1, -1, -1, 1, 1, 1, 1, 0), // right-bottom
                Engine::Render::Vertex(-1, -1, 1, 1, 1, 1, 0, 1), // left-top

                // top face
                // left triangle
                Engine::Render::Vertex(-1, 1, 1, 1, 1, 1, 0, 1),  // left-top
                Engine::Render::Vertex(-1, 1, -1, 1, 1, 1, 0, 0), // left-bottom
                Engine::Render::Vertex(1, 1, -1, 1, 1, 1, 1, 0),  // right-bottom

                // right triangle
                Engine::Render::Vertex(1, 1, 1, 1, 1, 1, 1, 1),  // right-top corner
                Engine::Render::Vertex(1, 1, -1, 1, 1, 1, 1, 0), // right-bottom
                Engine::Render::Vertex(-1, 1, 1, 1, 1, 1, 0, 1), // left-top

                // left face
                // left triangle
                Engine::Render::Vertex(-1, 1, 1, 1, 1, 1, 0, 1),   // left-top
                Engine::Render::Vertex(-1, -1, 1, 1, 1, 1, 0, 0),  // left-bottom
                Engine::Render::Vertex(-1, -1, -1, 1, 1, 1, 1, 0), // right-bottom

                // right triangle
                Engine::Render::Vertex(-1, 1, -1, 1, 1, 1, 1, 1),  // right-top corner
                Engine::Render::Vertex(-1, -1, -1, 1, 1, 1, 1, 0), // right-bottom
                Engine::Render::Vertex(-1, 1, 1, 1, 1, 1, 0, 1),   // left-top

                // right face
                // left triangle
                Engine::Render::Vertex(1, 1, 1, 1, 1, 1, 0, 1),   // left-top
                Engine::Render::Vertex(1, -1, 1, 1, 1, 1, 0, 0),  // left-bottom
                Engine::Render::Vertex(1, -1, -1, 1, 1, 1, 1, 0), // right-bottom

                // right triangle
                Engine::Render::Vertex(1, 1, -1, 1, 1, 1, 1, 1),  // right-top corner
                Engine::Render::Vertex(1, -1, -1, 1, 1, 1, 1, 0), // right-bottom
                Engine::Render::Vertex(1, 1, 1, 1, 1, 1, 0, 1),   // left-top

                // front face
                // left triangle
                Engine::Render::Vertex(-1, 1, 1, 1, 1, 1, 0, 1),  // left-top
                Engine::Render::Vertex(-1, -1, 1, 1, 1, 1, 0, 0), // left-bottom
                Engine::Render::Vertex(1, -1, 1, 1, 1, 1, 1, 0),  // right-bottom

                // right triangle
                Engine::Render::Vertex(1, 1, 1, 1, 1, 1, 1, 1),  // right-top corner
                Engine::Render::Vertex(1, -1, 1, 1, 1, 1, 1, 0), // right-bottom
                Engine::Render::Vertex(-1, 1, 1, 1, 1, 1, 0, 1), // left-top

                // back face
                // left triangle
                Engine::Render::Vertex(-1, 1, -1, 1, 1, 1, 0, 1),  // left-top
                Engine::Render::Vertex(-1, -1, -1, 1, 1, 1, 0, 0), // left-bottom
                Engine::Render::Vertex(1, -1, -1, 1, 1, 1, 1, 0),  // right-bottom

                // right triangle
                Engine::Render::Vertex(1, 1, -1, 1, 1, 1, 1, 1),  // right-top corner
                Engine::Render::Vertex(1, -1, -1, 1, 1, 1, 1, 0), // right-bottom
                Engine::Render::Vertex(-1, 1, -1, 1, 1, 1, 0, 1), // left-top
            };
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

        float speed = 10 * Engine::Render::GlobalRenderer->DeltaTime;
        Transform.Rotate(1 * speed, glm::vec3(Engine::Core::Random::Float(0, 1) * speed, Engine::Core::Random::Float(0, 1) * speed, Engine::Core::Random::Float(0, 1) * speed)); // rotate randomly
    }

    std::string FriendlyName()
    {
        return "MyPlane";
    }
};

class MyCube : public Engine::ECS::GameObject
{
public:
    void Start()
    {
        AddComponent(new CubeRenderer)->Public.SetString("texturePath", "bricks.jpg")->SetBool("flushTexture", true); // add the renderer and then set the texture path and flush the texture
    }

    void Update()
    {
        Engine::ECS::GameObject::Update(); // update components

        float speed = 10 * Engine::Render::GlobalRenderer->DeltaTime;
        Transform.Rotate(1 * speed, glm::vec3(Engine::Core::Random::Float(0, 1) * speed, Engine::Core::Random::Float(0, 1) * speed, Engine::Core::Random::Float(0, 1) * speed)); // rotate randomly
    }

    std::string FriendlyName()
    {
        return "MyCube";
    }
};

void Engine::Core::Application::Start()
{
    Engine::ECS::GlobalGameObjectManager->Add(new MyCube);
    Engine::Render::GlobalRenderer->CameraTransform.Translate(glm::vec3(0, 0.5, -5)); // translate the camera up and back
}

void Engine::Core::Application::Update()
{
}