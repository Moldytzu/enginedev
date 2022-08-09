#pragma once
#include <engine/vendor/glm/common.hpp>
#include <engine/vendor/glm/fwd.hpp>
#include <engine/core.h>
#include <vector>
#include <string>

namespace Engine::ECS
{
    class Component
    {
    public:
        virtual void Start();
        virtual void Update();

        virtual std::string FriendlyName();

        Engine::Core::Any Public;
    };

    class GameObject
    {
    public:
        virtual void Start();
        virtual void Update();

        virtual std::string FriendlyName();

        Component *AddComponent(Component *component);
        Component *DeleteComponent(Component *component);
        
        Component *GetComponent(std::string friendlyName);

        std::string Name;
        glm::vec3 Position;
        std::vector<Component *> Components;
    };

    class GameObjectManager
    {
    public:
        GameObject *Add(GameObject *obj);
        GameObject *Add(GameObject *obj, std::string name);

        void Delete(GameObject *obj);
        void Update();

        GameObject *Get(std::string name);
        bool Exists(std::string name);

    private:
        std::vector<GameObject *> gameobjects;
    };

    inline GameObjectManager *GlobalGameObjectManager;
};