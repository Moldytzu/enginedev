#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>
#include <engine/input.h>

int main(int argc, char *argv[])
{
    Engine::Core::Logger::LogInfo("Starting up the engine");

    // set the seed for rng
    srand(time(nullptr));

    // instantiate the required classes
    Engine::Core::ThreadManager threadManager;        // main thread manager (9100)
    Engine::Core::Application application;            // implemented by the application that uses this engine
    Engine::Render::Renderer renderer;                // main opengl 3 renderer
    Engine::ECS::GameObjectManager gameObjectManager; // main game object manager
    Engine::Input::InputManager inputManager;         // main input manager

    // set their address in the global scope
    Engine::ECS::GlobalGameObjectManager = &gameObjectManager;
    Engine::Render::GlobalRenderer = &renderer;
    Engine::Core::GlobalThreadManager = &threadManager;
    Engine::Input::GlobalInputManager = &inputManager;

    renderer.Init();     // initialise the renderer
    inputManager.Init(); // initialise the input manager
    application.Start(); // start the application

    while (renderer.Open())
    {
        renderer.ExecuteGraphics([&renderer]() -> const auto{ renderer.StartFrame(); });
        application.Update();       // update the application on every tick
        inputManager.Update();      // poll input
        gameObjectManager.Update(); // update the objects
        threadManager.Wait();       // wait for the threads to end their jobs
        renderer.ExecuteGraphics([&renderer]() -> const auto{ renderer.EndFrame(); });
    }

    threadManager.Wait();

    Engine::Core::Logger::LogInfo("Goodbye");
    return 0;
}