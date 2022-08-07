#define STB_IMAGE_IMPLEMENTATION

#include <engine/vendor/stb_image.h>
#include <engine/render.h>
#include <engine/core.h>
#include <engine/vendor/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>

GLFWwindow *window;
double lastTime;

std::string vertex =
    "#version 330 core\n"
    "layout (location = 0) in vec3 inVertexPosition;\n"
    "layout (location = 1) in vec3 inVertexColour;\n"
    "layout (location = 2) in vec2 inTexturePosition;\n"
    "out vec3 vertexColour;\n"
    "out vec2 texturePosition;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(inVertexPosition, 1.0);\n"
    "   vertexColour = inVertexColour;\n"
    "   texturePosition = inTexturePosition;\n"
    "}\0";

std::string fragment =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 vertexColour;\n"
    "in vec2 texturePosition;\n"
    "uniform sampler2D texture1;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture1, texturePosition) * vec4(vertexColour, 1.0);\n"
    "}\n\0";

void resize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // reset the viewport
}

void Engine::Render::Renderer::Init()
{
    Engine::Core::Logger::LogDebug("Initialising the renderer");

    glfwInit();                                    // initialise glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // hint that we want an opengl 3.3 core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(640, 480, "Main Window", NULL, NULL); // create the main window
    if (window == NULL)
    {
        Engine::Core::Logger::LogError("Failed to create window");
        glfwTerminate();
    }
    glfwMakeContextCurrent(window); // select the context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // load glad
    {
        Engine::Core::Logger::LogError("Failed to load GLAD");
        glfwTerminate();
    }

    glfwSetFramebufferSizeCallback(window, resize); // resize callback

    glEnable(GL_DEPTH);                   // depth checking
    glViewport(0, 0, 640, 480);           // set the viewport
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // grayish colour

    // create first empty texture
    unsigned int texture;
    glGenTextures(1, &texture);

    // compile the default shader
    DefaultFragmentShader = fragment;
    DefaultVertexShader = vertex;
    DefaultShader = CompileShader(DefaultVertexShader, DefaultFragmentShader);

    glUseProgram(DefaultShader);

    // grab the location of the uniforms
    viewLocation = glGetUniformLocation(DefaultShader, "view");
    modelLocation = glGetUniformLocation(DefaultShader, "model");
    projectionLocation = glGetUniformLocation(DefaultShader, "projection");

    // calculate the perspective
    CameraProjection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f); // generate projection
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(CameraProjection));           // set the projection

    glfwSwapInterval(0); // disable VSYNC
}

void Engine::Render::Renderer::StartFrame()
{
    lastTime = glfwGetTime();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window and the depth buffer

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(CameraTransform.Matrix)); // set the camera transform matrix
}

void Engine::Render::Renderer::EndFrame()
{
    DeltaTime = glfwGetTime() - lastTime; // determine time needed to draw a frame

    if (glfwGetTime() - (int)glfwGetTime() <= 0.01f) // update the title roughfly every second
    {
        std::string title = std::to_string((int)(1 / DeltaTime)) + " FPS"; // create the title
        glfwSetWindowTitle(window, title.c_str());                         // set the title
    }

    glfwSwapBuffers(window); // swap the buffers
    glfwPollEvents();        // poll for the events
}

unsigned int Engine::Render::Renderer::CompileShader(std::string vertex, std::string fragment)
{
    // create shaders for vertex and fragment
    const char *vertexSrc = vertex.c_str(), *fragmentSrc = fragment.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER), fragmentShader = glCreateShader(GL_FRAGMENT_SHADER), shaderProgram = glCreateProgram();

    // pass the source code and compile
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    // handle errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        Engine::Core::Logger::LogError("Failed to compile vertex shader! (" + std::string(infoLog) + ")");
        return -1;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        Engine::Core::Logger::LogError("Failed to compile fragment shader! (" + std::string(infoLog) + ")");
        return -1;
    }

    // link the shaders into a program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        Engine::Core::Logger::LogError("Failed to link shader! (" + std::string(infoLog) + ")");
        return -1;
    }

    // delete unused objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    shaders.push_back(shaderProgram);

    return shaderProgram;
}

unsigned int Engine::Render::Renderer::LoadTexture(std::string path)
{
    unsigned int texture;
    glGenTextures(1, &texture);                                   // generate a new texture                                 // generate texture
    glBindTexture(GL_TEXTURE_2D, texture);                        // bind texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // linear (more blurry and slower) mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    std::string parsedPath = Engine::Core::Filesystem::ParseRelativePath(path); // parse relative path

    unsigned char *data = stbi_load(parsedPath.c_str(), &width, &height, &nrChannels, 0); // load image
    if (!data)                                                                            // check for data
    {
        Engine::Core::Logger::LogError("Failed to load texture from " + path);
        stbi_image_free(data);
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // send data
    glGenerateMipmap(GL_TEXTURE_2D);                                                          // generate mipmap

    stbi_image_free(data); // free image data
    return texture;        // return the texture
}

Engine::Render::VertexBuffers Engine::Render::Renderer::GenerateBuffers(std::vector<Vertex> vertices, unsigned int shader, unsigned int texture)
{
    size_t size = vertices.size() * 8 * sizeof(float); // size in bytes of the data stored in the vertices

    float *verts = new float[vertices.size() * 8]; // raw array that stores the raw data
    for (size_t s = 0; s < vertices.size(); s++)   // fill the array
    {
        verts[s * 8 + 0] = vertices[s].x;
        verts[s * 8 + 1] = vertices[s].y;
        verts[s * 8 + 2] = vertices[s].z;
        verts[s * 8 + 3] = vertices[s].r;
        verts[s * 8 + 4] = vertices[s].g;
        verts[s * 8 + 5] = vertices[s].b;
        verts[s * 8 + 6] = vertices[s].tx;
        verts[s * 8 + 7] = vertices[s].ty;
    }

    Engine::Render::VertexBuffers buffers;
    glGenVertexArrays(1, &buffers.VAO); // generate vao and vbo
    glGenBuffers(1, &buffers.VBO);
    glBindVertexArray(buffers.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);
    glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW); // fill vbo with the vertex data

    // tell opengl how to handle the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    // enable the attributes
    for (int i = 0; i <= 2; i++)
        glEnableVertexAttribArray(i);

    // fill the structure
    buffers.shader = shader;
    buffers.vertices = size / sizeof(float) / 8; // get count of vertices
    buffers.texture = texture;

    return buffers;
}

Engine::Render::VertexBuffers Engine::Render::Renderer::GenerateBuffers(std::vector<Vertex> vertices, unsigned int shader)
{
    return GenerateBuffers(vertices, shader, 0);
}

Engine::Render::VertexBuffers Engine::Render::Renderer::GenerateBuffers(std::vector<Vertex> vertices)
{
    return GenerateBuffers(vertices, DefaultShader);
}

void Engine::Render::Renderer::Draw(Engine::Render::VertexBuffers buffer, Engine::Render::Transform transform)
{
    buffer.Bind();                                                                    // bind the buffers
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transform.Matrix)); // pass the matrix of the transform
    glDrawArrays(GL_TRIANGLES, 0, buffer.vertices);                                   // draw
}

bool Engine::Render::Renderer::Open()
{
    return !glfwWindowShouldClose(window); // returns true when the window shouldn't close / when it's open
}

Engine::Render::Renderer::~Renderer()
{
    Engine::Core::Logger::LogDebug("Destroying the renderer");
    glfwTerminate();
}