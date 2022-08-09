#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <engine/vendor/glm/common.hpp>
#include <engine/vendor/glm/fwd.hpp>

#define CHECK_CLASS { if (this == nullptr) { Engine::Core::Logger::LogError(std::string("Null class accessed! (") + __FILE__ + ":" + std::to_string(__LINE__) + ")"); exit(-1); } }

namespace Engine::Core
{
    class Any
    {
    public:
        Any *SetString(std::string key, std::string value);
        Any *SetInt(std::string key, int value);
        Any *SetFloat(std::string key, float value);
        Any *SetBool(std::string key, bool value);
        Any *SetVec3(std::string key, glm::vec3 value);

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

    class Logger
    {
    public:
        static void LogInfo(std::string msg);
        static void LogWarn(std::string msg);
        static void LogError(std::string msg);
        static void LogDebug(std::string msg);
    };

    class Filesystem
    {
    public:
        static std::string ParseRelativePath(std::string relative);
    };

    class Random
    {
    public:
        static int Int(int low, int high);
        static float Float(float low, float high);
    };

};
