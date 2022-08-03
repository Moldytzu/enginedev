#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <engine/vendor/glm/common.hpp>
#include <engine/vendor/glm/fwd.hpp>

namespace Engine::Core
{
    class Any
    {
    public:
        void SetString(std::string key, std::string value);
        void SetInt(std::string key, int value);
        void SetFloat(std::string key, float value);
        void SetBool(std::string key, bool value);
        void SetVec3(std::string key, glm::vec3 value);

        std::string GetString(std::string key);
        int GetInt(std::string key);
        float GetFloat(std::string key);
        bool GetBool(std::string key);
        glm::vec3 GetVec3(std::string key);

    private:
        std::unordered_map<std::string, std::any> map;
    };

    class Application
    {
    public:
        void Start();
        void Update();
    };

};
