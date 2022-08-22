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
    LOCK;
    Matrix = glm::translate(Matrix, glm::vec3(-offset.x, -offset.y, offset.z)); // translate with an offset
    UNLOCK;
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Scale(glm::vec3 axisFactor)
{
    LOCK;
    Matrix = glm::scale(Matrix, axisFactor); // scale
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