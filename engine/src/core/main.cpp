#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>

int main(int argc, char *argv[])
{
    Engine::Core::Logger::LogInfo("Starting up the engine");

    // instantiate the required classes
    Engine::Core::Application application; // this class is implemented by the application that uses this engine
    Engine::Render::Renderer renderer;
    Engine::ECS::GameObjectManager manager;

    // set their address in the global scope
    Engine::ECS::GlobalGameObjectManager = &manager;
    Engine::Render::GlobalRenderer = &renderer;

    renderer.Init();     // initialise the renderer
    application.Start(); // start the application

    while (renderer.Open())
    {
        renderer.StartFrame(); // create new frame
        application.Update();  // update the application on every tick
        manager.Update();      // update the objects
        renderer.EndFrame();   // terminate the frame
    }

    return 0;
}