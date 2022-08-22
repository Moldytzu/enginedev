#include <engine/core.h>
#include <iostream>

Engine::Core::Any::Any()
{
    UNLOCK;
}

Engine::Core::Any *Engine::Core::Any::SetString(std::string key, std::string value)
{
    LOCK;
    map[key] = value;
    UNLOCK;
    return this;
}

Engine::Core::Any *Engine::Core::Any::SetInt(std::string key, int value)
{
    LOCK;
    map[key] = value;
    UNLOCK;
    return this;
}

Engine::Core::Any *Engine::Core::Any::SetFloat(std::string key, float value)
{
    LOCK;
    map[key] = value;
    UNLOCK;
    return this;
}

Engine::Core::Any *Engine::Core::Any::SetBool(std::string key, bool value)
{
    LOCK;
    map[key] = value;
    UNLOCK;
    return this;
}

Engine::Core::Any *Engine::Core::Any::SetVec3(std::string key, glm::vec3 value)
{
    LOCK;
    map[key] = value;
    UNLOCK;
    return this;
}

std::string Engine::Core::Any::GetString(std::string key)
{
    std::string ret = "";
    try
    {
        LOCK;
        ret = std::any_cast<std::string>(map[key]);
        UNLOCK;
    }
    catch (std::bad_any_cast e)
    {
        Engine::Core::Logger::LogError("An exception happened when casting a key to string" + std::string(e.what()) + ")");
    }
    return ret;
}

int Engine::Core::Any::GetInt(std::string key)
{
    int ret = 0;
    try
    {
        LOCK;
        ret = std::any_cast<int>(map[key]);
        UNLOCK;
    }
    catch (std::bad_any_cast e)
    {
        Engine::Core::Logger::LogError("An exception happened when casting a key to int" + std::string(e.what()) + ")");
    }

    return ret;
}

float Engine::Core::Any::GetFloat(std::string key)
{
    float ret = 0;
    try
    {
        LOCK;
        ret = std::any_cast<float>(map[key]);
        UNLOCK;
    }
    catch (std::bad_any_cast e)
    {
        Engine::Core::Logger::LogError("An exception happened when casting a key to float" + std::string(e.what()) + ")");
    }
    return ret;
}

bool Engine::Core::Any::GetBool(std::string key)
{
    bool ret = false;
    try
    {
        LOCK;
        ret = std::any_cast<bool>(map[key]);
        UNLOCK;
    }
    catch (std::bad_any_cast e)
    {
        Engine::Core::Logger::LogError("An exception happened when casting a key to bool" + std::string(e.what()) + ")");
    }
    return ret;
}

glm::vec3 Engine::Core::Any::GetVec3(std::string key)
{
    glm::vec3 ret = {0, 0, 0};
    try
    {
        LOCK;
        ret = std::any_cast<glm::vec3>(map[key]);
        UNLOCK;
    }
    catch (std::bad_any_cast e)
    {
        Engine::Core::Logger::LogError("An exception happened when casting a key to vec3" + std::string(e.what()) + ")");
    }
    return ret;
}