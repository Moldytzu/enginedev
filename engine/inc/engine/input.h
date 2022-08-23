#include <mutex>
#include <engine/core.h>
#include <engine/render.h>

namespace Engine::Input
{
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        void Update();

        bool *Keys;
    private:
        std::mutex mutex;
    };

    inline InputManager *GlobalInputManager;
};