#include <engine/render.h>
#include <engine/core.h>
#include <engine/vendor/glm/gtx/matrix_decompose.hpp>

#undef LOCK
#undef UNLOCK

#define LOCK
#define UNLOCK

Engine::Render::Transform::Transform() : Matrix{glm::mat4(1.0f)} {UNLOCK;} // create a generic matrix

Engine::Render::Transform *Engine::Render::Transform::Translate(glm::vec3 offset)
{
    if(this == &Engine::Render::GlobalRenderer->CameraTransform) // the coordonates of the camera are kinda reversed
        offset = glm::vec3(-offset.x, -offset.y, offset.z);
    else
        offset.z = -offset.z;

    LOCK;
    // manualy translate
    Matrix[3][0] += offset.x;
    Matrix[3][1] += offset.y;
    Matrix[3][2] += offset.z;
    UNLOCK;
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Scale(glm::vec3 axisFactor)
{
    LOCK;
    // manualy scale because the glm function affects the translation
    Matrix[0][0] += axisFactor.x;
    Matrix[1][1] += axisFactor.y;
    Matrix[2][2] += axisFactor.z;
    UNLOCK;
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Rotate(float degrees, glm::vec3 axis)
{
    LOCK;
    Matrix = glm::rotate(Matrix, glm::radians(degrees), glm::normalize(axis)); // glm works only with radians
    UNLOCK;
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Reset()
{
    LOCK;
    Matrix = glm::mat4(1); // reset the matrix to its defaults
    UNLOCK;
    return this;
}