#pragma once
#include <engine/core.h>
#include <engine/render.h>
#include <engine/ecs.h>

namespace Engine::Standard::Components
{
    class BaseRenderer : public Engine::ECS::Component
    {
    public:
        Engine::Render::VertexBuffers buffers;
        std::vector<Engine::Render::Vertex> vertices;

        ~BaseRenderer()
        {
            Engine::Core::Logger::LogDebug("Destroying renderer component");
            Engine::Render::GlobalRenderer->ExecuteGraphics([&]() -> const auto{ buffers.Destroy(); });
        }

        virtual void Start()
        {
            // initial public variables states
            Public.SetString("texturePath", "");   // path to the texture
            Public.SetBool("flushTexture", false); // flag to use when flushing texture and loading a new one
        }
    
        virtual void Update()
        {
            // generate buffers if flush is requested
            if (Public.GetBool("flushTexture"))
            {
                buffers = Engine::Render::GlobalRenderer->GenerateBuffers(vertices, Engine::Render::GlobalRenderer->DefaultShader, Engine::Render::GlobalRenderer->LoadTexture(Public.GetString("texturePath"))); // generate new buffers
                Public.SetBool("flushTexture", false);                                                                                                                                                            // reset flag state
            }

            Engine::Render::GlobalRenderer->Draw(buffers, Parent->Transform); // draw the vertices
        }

        virtual std::string FriendlyName()
        {
            return "BaseRenderer";
        }
    };
};