#include <engine/core.h>
#include <iostream>

#define DEBUG

void Engine::Core::Logger::LogInfo(std::string msg)
{
    std::cout << "[INFO] " << msg << std::endl;
}

void Engine::Core::Logger::LogWarn(std::string msg)
{
    std::cout << "[WARN] " << msg << std::endl;
}

void Engine::Core::Logger::LogError(std::string msg)
{
    std::cout << "[ERROR] " << msg << std::endl;
}

void Engine::Core::Logger::LogDebug(std::string msg)
{
#ifdef DEBUG
    std::cout << "[DEBUG] " << msg << std::endl;
#endif
}