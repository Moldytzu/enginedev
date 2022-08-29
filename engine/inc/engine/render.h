#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <functional>
#include <thread>
#include <queue>
#include <engine/vendor/glad/glad.h>
#include <engine/vendor/glm/glm.hpp>
#include <engine/vendor/glm/gtc/matrix_transform.hpp>
#include <engine/vendor/glm/gtc/type_ptr.hpp>
#include <engine/vendor/glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::Render
{
    class Transform
    {
    public:
        glm::vec3 Translation;
        glm::vec3 Scalation;
        glm::vec3 Rotation;

        Transform();

        glm::mat4 Construct();

        Transform *Translate(glm::vec3 offset);
        Transform *Scale(glm::vec3 axisFactor);
        Transform *Rotate(float yaw, float pitch, float roll);

        Transform(const Transform &obj) : Translation{obj.Translation}, Scalation{obj.Scalation}, Rotation{obj.Rotation} {}

        Transform &operator=(Transform &obj)
        {
            Translation = obj.Translation;
            Scalation = obj.Scalation;
            Rotation = obj.Rotation;
            return *this;
        }

        std::mutex mutex;
    };

    struct VertexBuffers
    {
        unsigned int VAO, VBO, shader, texture;
        size_t vertices;

        void Destroy()
        {
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

    struct __Draw_Object
    {
        VertexBuffers vb;
        Transform t;
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

        VertexBuffers GenerateBuffers(std::vector<Vertex>& vertices, unsigned int shader, unsigned int texture);
        VertexBuffers GenerateBuffers(std::vector<Vertex>& vertices, unsigned int shader);
        VertexBuffers GenerateBuffers(std::vector<Vertex>& vertices);

        void ExecuteGraphics(const std::function<void()>& command);
        void WaitCompletion();
        void Draw(VertexBuffers buffer, Transform transform);

        Renderer();
        ~Renderer();

        std::string DefaultVertexShader, DefaultFragmentShader;
        unsigned int DefaultShader;

        Transform CameraTransform;
        glm::mat4 CameraProjection;

        double DeltaTime;

        unsigned int modelLocation, projectionLocation, viewLocation;

        GLFWwindow *Window;
    private:
        void threadInit();
        void threadLoop();

        std::thread thread;
        std::vector<int> shaders;
        std::vector<__Draw_Object> renderQueue;
        std::queue<std::function<void()>> jobs;
    };

    inline Renderer *GlobalRenderer;

};