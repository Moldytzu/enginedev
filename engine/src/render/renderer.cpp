#define STB_IMAGE_IMPLEMENTATION

#include <engine/vendor/stb_image.h>
#include <engine/render.h>
#include <engine/core.h>
#include <engine/vendor/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>

Engine::Render::Renderer *instance = nullptr; // instance of this
GLFWwindow *window = nullptr;                 // stores the glfw context
unsigned int ssFBO, rBO, tcBO;                // super sampling frame buffer object, rendering buffer object, texture color buffer object
int windowWidth, windowHeight;                // stores the current window size
int drawWidth, drawHeight;                    // stores the current rendering framebuffer size

// settings (TODO: move this in a class)
float fov = 90.0f;                             // field of view
bool superSampling = true;                     // toggle for super sampling
int superSamplingFactor = 2;                   // draw at 2x resolution
bool fixedAspectRatio = false;                 // toggle for fixed aspect ratio used in perspective calculation
float fixedAspectRatioValue = (float)(16 / 9); // fix to 16:9
bool anistropicFiltering = true;               // toggle for anistropic filtering
float targetAnistropicFiltering = 16.0f;       // factor for anistropic filtering
float maxAnistropicFiltering;                  // to be filled by the driver

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

void recreateOffScreenBuffer()
{
    static bool generated = false;
    if (generated == true)
    {
        glDeleteTextures(1, &tcBO);      // delete the color attachment texture
        glDeleteRenderbuffers(1, &rBO);  // delete the render buffer object
        glDeleteFramebuffers(1, &ssFBO); // delete the frame buffer
    }

    // generate a new framebuffer
    glGenFramebuffers(1, &ssFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssFBO);

    // create a color attachment texture
    glGenTextures(1, &tcBO);
    glBindTexture(GL_TEXTURE_2D, tcBO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, drawWidth, drawHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tcBO, 0);

    // create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &rBO);
    glBindRenderbuffer(GL_RENDERBUFFER, rBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, drawWidth, drawHeight);           // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rBO); // now actually attach it

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Engine::Core::Logger::LogError("Failed to create off-screen super-sampling framebuffer object! Disabling super sampling.");
        superSampling = false;                                               // disable super sampling
        drawHeight /= superSamplingFactor, drawWidth /= superSamplingFactor; // set the draw resolution to the window's resolution
    }

    // bind back the default draw buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    generated = true; // set the flag used to detect if we already created a buffer
}

void resize(GLFWwindow *window, int width, int height)
{
    windowWidth = drawWidth = width, windowHeight = drawHeight = height;

    if (superSampling)
    {
        drawHeight *= superSamplingFactor, drawWidth *= superSamplingFactor; // multiply the resolution by the super sampling factor
        recreateOffScreenBuffer();                                           // recreate the offscreen buffer
    }

    glViewport(0, 0, drawWidth, drawHeight); // reset the viewport

    // calculate the perspective
    float ratio = drawWidth / drawHeight;
    if (fixedAspectRatio)
        ratio = fixedAspectRatioValue;

    instance->CameraProjection = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);                     // generate projection
    glUniformMatrix4fv(instance->projectionLocation, 1, GL_FALSE, glm::value_ptr(instance->CameraProjection)); // set the projection
}

Engine::Render::Renderer::Renderer()
{
    mutex.unlock();
}

void Engine::Render::Renderer::Init()
{
    Engine::Core::Logger::LogDebug("Initialising the renderer");

    instance = this;

    glfwInit();                                    // initialise glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // hint that we want an opengl 3.3 core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    windowWidth = 640, windowHeight = 480;
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

    glfwSwapInterval(0); // disable vertical sync

    glEnable(GL_DEPTH_TEST); // enable depth testing

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

    resize(window, windowWidth, windowHeight); // simulate a resize event to regenerate the super sampling framebuffer and to properly calculate the perspective
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);      // grayish colour

    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnistropicFiltering); // determine max anistropic filtering factor
}

double currentFrame, lastFrame, updateTitleTimer = 0;

void Engine::Render::Renderer::StartFrame()
{
    // delta time calculation
    currentFrame = glfwGetTime();
    DeltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (currentFrame >= updateTitleTimer + 1) // update the title roughfly every second
    {
        updateTitleTimer = currentFrame;                                   // reset timer
        std::string title = std::to_string((int)(1 / DeltaTime)) + " FPS"; // create the title
        glfwSetWindowTitle(window, title.c_str());                         // set the title
    }

    // clear the render queue
    renderQueue.clear();

    glfwMakeContextCurrent(window);

    // use the super sampling framebuffer if enabled
    if (superSampling)
        glBindFramebuffer(GL_FRAMEBUFFER, ssFBO);
    else
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // enable anisotropic filtering if enabled
    float anisoValue = glm::min(targetAnistropicFiltering, maxAnistropicFiltering);
    if (!anistropicFiltering)
        anisoValue = 1.0f;

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoValue);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window and the depth buffer

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(CameraTransform.Matrix)); // set the camera transform matrix
}

void Engine::Render::Renderer::EndFrame()
{
    // render everything
    for (int i = 0; i < renderQueue.size(); i++)
    {
        __Draw_Object obj = renderQueue[i];
        obj.vb.Bind();                                                                // bind the buffers
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(obj.t.Matrix)); // pass the matrix of the transform
        glDrawArrays(GL_TRIANGLES, 0, obj.vb.vertices);                               // draw
    }

    if (superSampling)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);     // bind the default framebuffer used to copy the super sampling frame buffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, ssFBO); // bind the super sampling framebuffer used to read the drawn stuff
        glBlitFramebuffer(0, 0, drawWidth, drawHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    glfwSwapBuffers(window); // swap the buffers
    glfwPollEvents();        // poll for the events
}

unsigned int Engine::Render::Renderer::CompileShader(std::string vertex, std::string fragment)
{
    LOCK;
    glfwMakeContextCurrent(window);
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
        UNLOCK;
        return -1;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        Engine::Core::Logger::LogError("Failed to compile fragment shader! (" + std::string(infoLog) + ")");
        UNLOCK;
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
        UNLOCK;
        return -1;
    }

    // delete unused objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    shaders.push_back(shaderProgram);
    UNLOCK;
    return shaderProgram;
}

unsigned int Engine::Render::Renderer::LoadTexture(std::string path)
{
    LOCK;
    glfwMakeContextCurrent(window);
    unsigned int texture;
    glGenTextures(1, &texture);                                   // generate a new texture
    glBindTexture(GL_TEXTURE_2D, texture);                        // bind texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // linear (more blurry and slower) mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    int width, height, nrChannels;

    std::string parsedPath = Engine::Core::Filesystem::ParseRelativePath(path); // parse relative path

    unsigned char *data = stbi_load(parsedPath.c_str(), &width, &height, &nrChannels, 0); // load image
    if (!data)                                                                            // check for data
    {
        Engine::Core::Logger::LogError("Failed to load texture from " + path);
        stbi_image_free(data);
        UNLOCK;
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // send data
    glGenerateMipmap(GL_TEXTURE_2D);                                                          // generate mipmap

    stbi_image_free(data); // free image data
    UNLOCK;
    return texture;        // return the texture
}

Engine::Render::VertexBuffers Engine::Render::Renderer::GenerateBuffers(std::vector<Vertex>& vertices, unsigned int shader, unsigned int texture)
{
    LOCK;
    glfwMakeContextCurrent(window);
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

    UNLOCK;
    return buffers;
}

Engine::Render::VertexBuffers Engine::Render::Renderer::GenerateBuffers(std::vector<Vertex>& vertices, unsigned int shader)
{
    return GenerateBuffers(vertices, shader, 0);
}

Engine::Render::VertexBuffers Engine::Render::Renderer::GenerateBuffers(std::vector<Vertex>& vertices)
{
    return GenerateBuffers(vertices, DefaultShader);
}

void Engine::Render::Renderer::Draw(Engine::Render::VertexBuffers buffer, Engine::Render::Transform transform)
{
    LOCK;
    __Draw_Object obj;                  // create instance of an internal object
    obj.t = transform, obj.vb = buffer; // set its metadata
    renderQueue.push_back(obj);         // push it on the render queue
    UNLOCK;
}

bool Engine::Render::Renderer::Open()
{
    return !glfwWindowShouldClose(window); // returns true when the window shouldn't close / when it's open
}

Engine::Render::Renderer::~Renderer()
{
    Engine::Core::Logger::LogDebug("Destroying the Renderer");
    glfwMakeContextCurrent(window);
    // delete the shaders
    for (size_t s = 0; s < shaders.size(); s++)
        glDeleteProgram(shaders[s]);

    glDeleteTextures(1, &tcBO);      // delete the color attachment texture
    glDeleteRenderbuffers(1, &rBO);  // delete the render buffer object
    glDeleteFramebuffers(1, &ssFBO); // delete the frame buffer

    glfwTerminate();
}