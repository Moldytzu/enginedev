#pragma once
#include <string>
#include <vector>
#include <engine/vendor/glad/glad.h>

namespace Engine::Render
{
    struct VertexBuffers
    {
        unsigned int VAO, VBO, shader;
        size_t vertices;

        void Bind()
        {
            glUseProgram(shader);
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

        VertexBuffers GenerateBuffers(float *vertices, unsigned int shader);
        VertexBuffers GenerateBuffers(float *vertices);

        void Draw(VertexBuffers buffer);

        ~Renderer();

        std::string DefaultVertexShader, DefaultFragmentShader;
        unsigned int DefaultShader;

    private:
        std::vector<int> shaders;
    };

    inline Renderer *GlobalRenderer;

};