#include <engine/render.h>
#include <engine/vendor/glm/gtx/matrix_decompose.hpp>

Engine::Render::Transform::Transform() : Matrix{glm::mat4(1.0f)} {} // create a generic matrix

Engine::Render::Transform *Engine::Render::Transform::Translate(glm::vec3 offset)
{
    Matrix = glm::translate(Matrix, glm::vec3(-offset.x, -offset.y, offset.z)); // translate with an offset
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Scale(glm::vec3 axisFactor)
{
    Matrix = glm::scale(Matrix, axisFactor); // scale
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Rotate(float degrees, glm::vec3 axis)
{
    Matrix = glm::rotate(Matrix, glm::radians(degrees), glm::normalize(axis)); // glm works only with radians
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Reset()
{
    Matrix = glm::mat4(1); // reset the matrix to its defaults
    return this;
}