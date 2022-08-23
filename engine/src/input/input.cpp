#include <engine/input.h>

Engine::Input::InputManager::InputManager()
{
    Engine::Core::Logger::LogDebug("Initialising the input manager");
    Keys = new bool[GLFW_KEY_LAST - GLFW_KEY_SPACE]; // create an array to store the key states
}

Engine::Input::InputManager::~InputManager()
{
    Engine::Core::Logger::LogDebug("Destroying the InputManager");
    delete Keys; // delete the array
}

void Engine::Input::InputManager::Update()
{
    LOCK;
    UNLOCK;
}