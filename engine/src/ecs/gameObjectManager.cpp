#include <engine/ecs.h>
#include <engine/core.h>

bool Engine::ECS::GameObjectManager::Exists(std::string name)
{
    for (int i = 0; i < gameobjects.size(); i++) // search for the object
        if (gameobjects[i]->Name == name)        // compare name
            return true;

    return false;
}

Engine::ECS::GameObject *Engine::ECS::GameObjectManager::Add(Engine::ECS::GameObject *obj)
{
    return Add(obj, obj->FriendlyName()); // don't use any fancy name, just the friendly name itself
}

Engine::ECS::GameObject *Engine::ECS::GameObjectManager::Add(Engine::ECS::GameObject *obj, std::string name)
{
    if (Exists(name)) // check if an object with that name exists
    {
        std::string tmp = name;
        for (int i = 1; Exists(tmp); i++)   // check if an object with that number and name exists
            tmp = name + std::to_string(i); // append next index

        name = tmp;
    }

    Engine::Core::Logger::LogDebug("Adding object with name " + name + " as " + obj->FriendlyName());

    obj->Name = name;           // set the name
    gameobjects.push_back(obj); // push the object
    obj->Start();               // start it
    return obj;
}

void Engine::ECS::GameObjectManager::Delete(Engine::ECS::GameObject *obj)
{
    for (int i = 0; i < gameobjects.size(); i++) // search for the object
    {
        if (gameobjects[i] == obj) // if found
        {
            Engine::Core::Logger::LogDebug("Erasing object with name " + gameobjects[i]->Name + " as " + obj->FriendlyName());
            gameobjects.erase(i + gameobjects.begin()); // erase it
            return;
        }
    }
}

void Engine::ECS::GameObjectManager::Update()
{
    for (int i = 0; i < gameobjects.size(); i++) // update every object
        gameobjects[i]->Update();
}

Engine::ECS::GameObject *Engine::ECS::GameObjectManager::Get(std::string name)
{
    for (int i = 0; i < gameobjects.size(); i++) // search for the object
        if (gameobjects[i]->Name == name)        // if found
            return gameobjects[i];               // return it
    return nullptr;
}