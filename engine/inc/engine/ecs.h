#pragma once
#include <engine/vendor/glm/common.hpp>
#include <engine/vendor/glm/fwd.hpp>
#include <engine/core.h>
#include <engine/render.h>
#include <vector>
#include <string>
#include <mutex>

namespace Engine::ECS
{
    class GameObject;

    class Component
    {
    public:
        virtual ~Component() = 0;

        virtual void Start();
        virtual void Update();

        virtual std::string FriendlyName();

        Engine::Core::Any Public;
        GameObject *Parent;    
    private:
        std::mutex mutex;
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

        ~GameObject();

        std::string Name;
        Engine::Render::Transform Transform;
        std::vector<Component *> Components;
    private:
        std::mutex mutex;
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

        GameObjectManager();
        ~GameObjectManager();
    private:
        std::vector<GameObject *> gameobjects;
        std::mutex mutex;
    };

    inline GameObjectManager *GlobalGameObjectManager;
};