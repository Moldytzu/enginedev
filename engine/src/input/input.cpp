#include <engine/input.h>

Engine::Input::InputManager *_instance;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Engine::Core::Logger::LogInfo(std::string(1,(char)key));
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