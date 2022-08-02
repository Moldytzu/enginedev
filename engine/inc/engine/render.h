#pragma once
#include <string>
#include <vector>
#include <engine/vendor/glad/glad.h>


namespace Engine::Render
{
    struct VertexBuffers
    {
        unsigned int VAO, VBO;

        void Bind()
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindVertexArray(VAO);
        }
    };

    class Renderer
    {
    public:
        void Init();
        void StartFrame();
        void EndFrame();

        bool Open();

        unsigned int CompileShader(std::string vertex, std::string fragment);

        VertexBuffers GenerateBuffers(float *vertices, size_t size);

        void Draw(VertexBuffers buffer, size_t vertices);

        ~Renderer();

    private:
        std::vector<int> shaders;
    };

    inline Renderer *GlobalRenderer;

};