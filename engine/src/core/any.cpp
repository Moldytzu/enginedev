#include <engine/core.h>

void Engine::Core::Any::SetString(std::string key, std::string value)
{
    map[key] = value;
}

void Engine::Core::Any::SetInt(std::string key, int value)
{
    map[key] = value;
}

void Engine::Core::Any::SetFloat(std::string key, float value)
{
    map[key] = value;
}

void Engine::Core::Any::SetBool(std::string key, bool value)
{
    map[key] = value;
}

void Engine::Core::Any::SetVec3(std::string key, glm::vec3 value)
{
    map[key] = value;
}

std::string Engine::Core::Any::GetString(std::string key)
{
    return std::any_cast<std::string>(map[key]);
}

int Engine::Core::Any::GetInt(std::string key)
{
    return std::any_cast<int>(map[key]);
}

float Engine::Core::Any::GetFloat(std::string key)
{
    return std::any_cast<float>(map[key]);
}

bool Engine::Core::Any::GetBool(std::string key)
{
    return std::any_cast<bool>(map[key]);
}

glm::vec3 Engine::Core::Any::GetVec3(std::string key)
{
    return std::any_cast<glm::vec3>(map[key]);
}