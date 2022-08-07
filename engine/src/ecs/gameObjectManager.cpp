#include <engine/ecs.h>

void Engine::ECS::GameObjectManager::Add(Engine::ECS::GameObject *obj)
{
    obj->Name = obj->FriendlyName(); // set its name to the friendly name. TODO: accept new name
    gameobjects.push_back(obj);      // push the object
    obj->Start();                    // start it
}

void Engine::ECS::GameObjectManager::Delete(Engine::ECS::GameObject *obj)
{
    for (int i = 0; i < gameobjects.size(); i++) // search for the object
    {
        if (gameobjects[i] == obj) // if found
        {
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