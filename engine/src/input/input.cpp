#include <engine/input.h>
#include <iostream>

Engine::Input::InputManager *_instance;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    bool pressed = (action == GLFW_PRESS) || (action == GLFW_REPEAT); // we treat the actions of GLFW_PRESS and GLFW_REPEAT as key down and GLFW_RELEASE as key up
    _instance->Keys[key] = pressed;                              // store the information
}

void Engine::Input::InputManager::Init()
{
    Engine::Core::Logger::LogDebug("Initialising the input manager");
    _instance = this;

    Keys = new bool[GLFW_KEY_LAST - GLFW_KEY_SPACE]; // create an array to store the key states

    window = Engine::Render::GlobalRenderer->Window; // set our internal glfw window to the renderer's one

    glfwSetKeyCallback(window, keyCallback); // set the callback for key input
}

Engine::Input::InputManager::InputManager()
{
    UNLOCK;
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