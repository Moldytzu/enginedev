#include <engine/ecs.h>
#include <engine/core.h>

Engine::ECS::GameObjectManager::GameObjectManager()
{
    mutex.unlock();
}

Engine::ECS::GameObjectManager::~GameObjectManager()
{
    Engine::Core::Logger::LogDebug("Destroying the GameObjectManager");

    for (int i = 0; i < gameobjects.size(); i++) // update every gameobject
        delete gameobjects[i];
}

bool Engine::ECS::GameObjectManager::Exists(std::string name)
{
    LOCK;
    for (int i = 0; i < gameobjects.size(); i++) // search for the object
    {
        if (gameobjects[i]->Name == name)        // compare name
        {
            UNLOCK;
            return true;
        }        
    }
    UNLOCK;
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

    LOCK;
    obj->Name = name;           // set the name
    gameobjects.push_back(obj); // push the object
    obj->Start();               // start it
    UNLOCK;
    return obj;
}

void Engine::ECS::GameObjectManager::Delete(Engine::ECS::GameObject *obj)
{
    LOCK;
    for (int i = 0; i < gameobjects.size(); i++) // search for the object
    {
        if (gameobjects[i] == obj) // if found
        {
            Engine::Core::Logger::LogDebug("Erasing object with name " + gameobjects[i]->Name + " as " + obj->FriendlyName());
            delete gameobjects[i];
            gameobjects.erase(i + gameobjects.begin()); // erase it
            UNLOCK;
            return;
        }
    }
    UNLOCK;
}

void Engine::ECS::GameObjectManager::Update()
{
    LOCK;
    for (int i = 0; i < gameobjects.size(); i++) // update every object
        Engine::Core::GlobalThreadManager->Queue([i, this]() -> const auto{ gameobjects[i]->Update(); });
    UNLOCK;
}

Engine::ECS::GameObject *Engine::ECS::GameObjectManager::Get(std::string name)
{
    LOCK;
    for (int i = 0; i < gameobjects.size(); i++) // search for the object
    {
        if (gameobjects[i]->Name == name)        // if found
        {
            UNLOCK;
            return gameobjects[i];               // return it               
        }
    }

    UNLOCK;
    return nullptr;
}