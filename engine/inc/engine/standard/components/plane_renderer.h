#pragma once
#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>
#include <engine/standard/components/base_renderer.h>

namespace Engine::Standard::Components
{
    class PlaneRenderer : public Engine::Standard::Components::BaseRenderer
    {
    public:
        void Start()
        {
            BaseRenderer::Start(); // initialise the base renderer class

            // vertices information
            vertices =
                {
                    // left triangle
                    Engine::Render::Vertex(-0.5f, 0, 0.5f, 1, 1, 1, 0, 1),  // left-top
                    Engine::Render::Vertex(-0.5f, 0, -0.5f, 1, 1, 1, 0, 0), // left-bottom
                    Engine::Render::Vertex(0.5f, 0, -0.5f, 1, 1, 1, 1, 0),  // right-bottom

                    // right triangle
                    Engine::Render::Vertex(0.5f, 0, 0.5f, 1, 1, 1, 1, 1),  // right-top corner
                    Engine::Render::Vertex(0.5f, 0, -0.5f, 1, 1, 1, 1, 0), // right-bottom
                    Engine::Render::Vertex(-0.5f, 0, 0.5f, 1, 1, 1, 0, 1), // left-top

                };
        }

        std::string FriendlyName()
        {
            return "PlaneRenderer";
        }
    };
};