#include <engine/render.h>

Engine::Render::Transform::Transform() : Matrix{glm::mat4(1.0f)} {} // create a generic matrix

void Engine::Render::Transform::Translate(glm::vec3 offset)
{
    Matrix = glm::translate(Matrix, offset); // translate with an offset
}