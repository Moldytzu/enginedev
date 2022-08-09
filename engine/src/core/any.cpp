#include <engine/core.h>
#include <iostream>

Engine::Core::Any *Engine::Core::Any::SetString(std::string key, std::string value)
{
    map[key] = value;
    return this;
}

Engine::Core::Any *Engine::Core::Any::SetInt(std::string key, int value)
{
    map[key] = value;
    return this;
}

Engine::Core::Any *Engine::Core::Any::SetFloat(std::string key, float value)
{
    map[key] = value;
    return this;
}

Engine::Core::Any *Engine::Core::Any::SetBool(std::string key, bool value)
{
    map[key] = value;
    return this;
}

Engine::Core::Any *Engine::Core::Any::SetVec3(std::string key, glm::vec3 value)
{
    map[key] = value;
    return this;
}

std::string Engine::Core::Any::GetString(std::string key)
{
    std::string ret = "";
    try
    {
        ret = std::any_cast<std::string>(map[key]);
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
        ret = std::any_cast<int>(map[key]);
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
        ret = std::any_cast<float>(map[key]);
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
        ret = std::any_cast<bool>(map[key]);
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
        ret = std::any_cast<glm::vec3>(map[key]);
    }
    catch (std::bad_any_cast e)
    {
        Engine::Core::Logger::LogError("An exception happened when casting a key to vec3" + std::string(e.what()) + ")");
    }
    return ret;
}