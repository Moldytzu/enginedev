#pragma once

namespace Engine::Render
{
    class Renderer
    {
    public:
        void Init();
        void Update();

        bool Open();

        ~Renderer();
    };

};