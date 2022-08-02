#include <engine/core.h>
#include <engine/render.h>

int main(int argc, char *argv[])
{
    Engine::Core::Application application; // this class is implemented by the application that uses this engine
    Engine::Render::Renderer renderer;

    renderer.Init();     // initialise th renderer
    application.Start(); // start the application

    while (renderer.Open())
    {
        renderer.StartFrame();
        application.Update(); // update it on every tick
        renderer.EndFrame();
    }

    return 0;
}