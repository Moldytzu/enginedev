#include <engine/ecs.h>
#include <engine/core.h>

void Engine::ECS::GameObject::Start() {}
std::string Engine::ECS::GameObject::FriendlyName() { return "Game Object"; }
void Engine::ECS::GameObject::Update()
{
    CHECK_CLASS;
    for (int i = 0; i < Components.size(); i++) // update every component
        Components[i]->Update();
}

Engine::ECS::Component *Engine::ECS::GameObject::AddComponent(Engine::ECS::Component *component)
{
    CHECK_CLASS;
    Engine::Core::Logger::LogDebug("Adding component " + component->FriendlyName() + " to " + Name);
    Components.push_back(component); // push the component
    component->Start();              // start it
    component->Parent = this;        // set the parent
    return component;
}

Engine::ECS::Component *Engine::ECS::GameObject::DeleteComponent(Component *component)
{
    CHECK_CLASS;
    if (component == nullptr) // don't fiddle with null addresses
        return component;

    for (int i = 0; i < Components.size(); i++) // itterate on every component
    {
        if (Components[i] == component)
        {
            Engine::Core::Logger::LogDebug("Deleting component " + component->FriendlyName() + " from " + Name);
            Components.erase(Components.begin() + i); // erase at the position
            delete component;                         // delete the component

            return component;
        }
    }

    return nullptr;
}

Engine::ECS::Component *Engine::ECS::GameObject::GetComponent(std::string friendlyName)
{
    CHECK_CLASS;
    for (int i = 0; i < Components.size(); i++) // itterate on every component
    {
        if (Components[i]->FriendlyName() == friendlyName) // compare names
        {
            Engine::Core::Logger::LogDebug("Returning component " + Components[i]->FriendlyName() + " from " + Name);
            return Components[i];
        }
    }

    return nullptr;
}