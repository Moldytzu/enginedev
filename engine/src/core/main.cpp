#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>

int main(int argc, char *argv[])
{
    Engine::Core::Application application; // this class is implemented by the application that uses this engine
    Engine::Render::Renderer renderer;
    Engine::ECS::GameObjectManager manager;

    Engine::ECS::GlobalGameObjectManager = &manager;
    Engine::Render::GlobalRenderer = &renderer;

    renderer.Init();     // initialise the renderer
    application.Start(); // start the application

    while (renderer.Open())
    {
        renderer.StartFrame();
        application.Update(); // update the application on every tick
        manager.Update();     // update the objects
        renderer.EndFrame();
    }

    return 0;
}