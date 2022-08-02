#include <engine/core.h>

int main(int argc, char *argv[])
{
    Engine::Core::Application application; // this class is implemented by the application that uses this engine

    application.Start(); // start the application
    
    while (true)
    {
        application.Update(); // update it on every tick
    }

    return 0;
}