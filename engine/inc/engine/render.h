#pragma once

namespace Engine::Render
{
    class Renderer
    {
    public:
        void Init();
        void StartFrame();
        void EndFrame();

        bool Open();

        ~Renderer();
    };

};