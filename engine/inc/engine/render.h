#pragma once
#include <string>
#include <vector>
#include <engine/vendor/glad/glad.h>
#include <engine/vendor/glm/glm.hpp>
#include <engine/vendor/glm/gtc/matrix_transform.hpp>
#include <engine/vendor/glm/gtc/type_ptr.hpp>

namespace Engine::Render
{
    class Transform
    {
    public:
        glm::mat4 Matrix;

        Transform();

        Transform *Translate(glm::vec3 offset);
        Transform *Scale(glm::vec3 axisFactor);
        Transform *Rotate(float degrees, glm::vec3 axis);

        Transform *Reset();
    };

    struct VertexBuffers
    {
        unsigned int VAO, VBO, shader, texture;
        size_t vertices;

        void Destroy()
        {
            glDeleteProgram(shader);
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteTextures(1, &texture);
        }

        void Bind()
        {
            glUseProgram(shader);                  // shader
            glBindBuffer(GL_ARRAY_BUFFER, VBO);    // vbo
            glBindVertexArray(VAO);                // vao
            glBindTexture(GL_TEXTURE_2D, texture); // texture
        }
    };

    class Vertex
    {
    public:
        float x, y, z, r, g, b, tx, ty;
        Vertex() : x{0}, y{0}, z{0}, r{0}, g{0}, b{0}, tx{0}, ty{0} {}
        Vertex(float x, float y, float z, float r, float g, float b, float tx, float ty) : x{x}, y{y}, z{z}, r{r}, g{g}, b{b}, tx{tx}, ty{ty} {}
        Vertex(float x, float y, float z, float r, float g, float b) : x{x}, y{y}, z{z}, r{r}, g{g}, b{b}, tx{0}, ty{0} {}
        Vertex(float x, float y, float z) : x{x}, y{y}, z{z}, r{1}, g{1}, b{1}, tx{0}, ty{0} {}
        Vertex(float x, float y, float r, float g, float b) : x{x}, y{y}, z{0}, r{r}, g{g}, b{b}, tx{0}, ty{0} {}
        Vertex(float x, float y) : x{x}, y{y}, z{0}, r{1}, g{1}, b{1}, tx{0}, ty{0} {}
    };

    class Renderer
    {
    public:
        void Init();
        void StartFrame();
        void EndFrame();

        bool Open();

        unsigned int CompileShader(std::string vertex, std::string fragment);

        unsigned int LoadTexture(std::string path);

        VertexBuffers GenerateBuffers(std::vector<Vertex> vertices, unsigned int shader, unsigned int texture);
        VertexBuffers GenerateBuffers(std::vector<Vertex> vertices, unsigned int shader);
        VertexBuffers GenerateBuffers(std::vector<Vertex> vertices);

        void Draw(VertexBuffers buffer, Transform transform);

        ~Renderer();

        std::string DefaultVertexShader, DefaultFragmentShader;
        unsigned int DefaultShader;

        Transform CameraTransform;
        glm::mat4 CameraProjection;

        double DeltaTime;

        unsigned int modelLocation, projectionLocation, viewLocation;

    private:
        std::vector<int> shaders;
    };

    inline Renderer *GlobalRenderer;

};