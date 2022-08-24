#pragma once
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

        void Init();
        void Update();

        bool *Keys;
    private:
        GLFWwindow *window;
        std::mutex mutex;
    };

    inline InputManager *GlobalInputManager;
};