#include <engine/ecs.h>

void Engine::ECS::GameObject::Start() {}
std::string Engine::ECS::GameObject::FriendlyName() { return "Game Object"; }
void Engine::ECS::GameObject::Update()
{
    for (int i = 0; i < Components.size(); i++) // update every component
        Components[i]->Update();
}

void Engine::ECS::GameObject::AddComponent(Engine::ECS::Component *component)
{
    Components.push_back(component); // push the component
    component->Start();              // start it
}