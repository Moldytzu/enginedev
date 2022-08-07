#include <engine/ecs.h>
#include <engine/core.h>

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

void Engine::ECS::GameObject::DeleteComponent(Component *component)
{
    if (component == nullptr) // don't fiddle with null addresses
        return;

    for (int i = 0; i < Components.size(); i++) // itterate on every component
    {
        if (Components[i] == component)
        {
            Components.erase(Components.begin() + i); // erase at the position
            delete component;                         // delete the component

            return;
        }
    }
}

Engine::ECS::Component *Engine::ECS::GameObject::GetComponent(std::string friendlyName)
{
    for (int i = 0; i < Components.size(); i++) // itterate on every component
    {
        Engine::Core::Logger::LogDebug(Components[i]->FriendlyName());
        if (Components[i]->FriendlyName() == friendlyName) // compare names
        {
            return Components[i];
        }
    }

    return nullptr;
}