#include <engine/core.h>
#include <engine/render.h>

int main(int argc, char *argv[])
{
    Engine::Core::Application application; // this class is implemented by the application that uses this engine
    Engine::Render::Renderer renderer;

    application.Start(); // start the application
    renderer.Init();

    while (renderer.Open())
    {
        renderer.Update();
        application.Update(); // update it on every tick
    }

    return 0;
}