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

    class Vertex
    {
    public:
        float x,y,z,r,g,b;
        Vertex() : x{0}, y{0}, z{0}, r{0}, g{0}, b{0} {}
        Vertex(float x, float y, float z, float r, float g, float b) : x{x}, y{y}, z{z}, r{r}, g{g}, b{b} {}
        Vertex(float x, float y, float z) : x{x}, y{y}, z{z}, r{1}, g{1}, b{1} {}
        Vertex(float x, float y, float r, float g, float b) : x{x}, y{y}, z{0}, r{r}, g{g}, b{b} {}
        Vertex(float x, float y) : x{x}, y{y}, z{0}, r{1}, g{1}, b{1} {}
    };

    class Renderer
    {
    public:
        void Init();
        void StartFrame();
        void EndFrame();

        bool Open();

        unsigned int CompileShader(std::string vertex, std::string fragment);

        VertexBuffers GenerateBuffers(std::vector<Vertex> vertices, unsigned int shader);
        VertexBuffers GenerateBuffers(std::vector<Vertex> vertices);

        void Draw(VertexBuffers buffer);

        ~Renderer();

        std::string DefaultVertexShader, DefaultFragmentShader;
        unsigned int DefaultShader;

    private:
        std::vector<int> shaders;
    };

    inline Renderer *GlobalRenderer;

};