#include <engine/render.h>

Engine::Render::Transform::Transform() : Matrix{glm::mat4(1.0f)} {} // create a generic matrix

void Engine::Render::Transform::Translate(glm::vec3 offset)
{
    Matrix = glm::translate(Matrix, glm::vec3(-offset.x, -offset.y, offset.z)); // translate with an offset
}