#pragma once
#include <engine/vendor/glm/common.hpp>
#include <engine/vendor/glm/fwd.hpp>
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
    };

    class GameObject
    {
    public:
        virtual void Start();
        virtual void Update();

        virtual std::string FriendlyName();

        void AddComponent(Component *component);
        void DeleteComponent(Component *component);
        
        Component *GetComponent(std::string friendlyName);

        std::string Name;
        glm::vec3 Position;
        std::vector<Component *> Components;
    };

    class GameObjectManager
    {
    public:
        void Add(GameObject *obj);
        void Add(GameObject *obj, std::string name);

        void Delete(GameObject *obj);
        void Update();

        GameObject *Get(std::string name);
        bool Exists(std::string name);

    private:
        std::vector<GameObject *> gameobjects;
    };

    inline GameObjectManager *GlobalGameObjectManager;
};