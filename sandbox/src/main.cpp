#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>
#include <engine/input.h>
#include <engine/standard/components/plane_renderer.h>
#include <engine/standard/components/cube_renderer.h>
#include <iostream>
#include <time.h>

class MyGround : public Engine::ECS::GameObject
{
public:
    void Start()
    {
        AddComponent(new Engine::Standard::Components::PlaneRenderer)->Public.SetString("texturePath", "bricks.jpg")->SetBool("flushTexture", true); // add the renderer and then set the texture path and flush the texture
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

class MyCube : public Engine::ECS::GameObject
{
public:
    void Start()
    {
        AddComponent(new Engine::Standard::Components::CubeRenderer)->Public.SetString("texturePath", "bricks.jpg")->SetBool("flushTexture", true); // add the renderer and then set the texture path and flush the texture
    }

    void Update()
    {
        Engine::ECS::GameObject::Update(); // update components

        if (Engine::Input::GlobalInputManager->Keys[GLFW_KEY_W])
        {
            float speed = 100 * Engine::Render::GlobalRenderer->DeltaTime;
            Transform.Rotate(1 * speed, glm::vec3(Engine::Core::Random::Float(0, 1) * speed, Engine::Core::Random::Float(0, 1) * speed, Engine::Core::Random::Float(0, 1) * speed)); // rotate randomly
        }
    }

    std::string FriendlyName()
    {
        return "MyCube";
    }
};

void Engine::Core::Application::Start()
{
    Engine::ECS::GlobalGameObjectManager->Add(new MyCube);
    Engine::ECS::GlobalGameObjectManager->Add(new MyGround);
    Engine::Render::GlobalRenderer->CameraTransform.Translate(glm::vec3(0, 1, -5)); // translate the camera up and back
}

void Engine::Core::Application::Update()
{
}