#include <engine/render.h>
#include <engine/core.h>
#include <engine/vendor/glm/gtx/euler_angles.hpp>
#include <engine/vendor/glm/gtx/matrix_decompose.hpp>

#undef LOCK
#undef UNLOCK

#define LOCK
#define UNLOCK

Engine::Render::Transform::Transform() : Translation{glm::vec3(1.0f)}, Rotation{glm::vec3(0)}, Scalation{glm::vec3(1)} {UNLOCK;} // create a generic matrix

Engine::Render::Transform *Engine::Render::Transform::Translate(glm::vec3 offset)
{
    LOCK;
    Translation += offset;
    UNLOCK;
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Scale(glm::vec3 axisFactor)
{
    LOCK;
    Scalation += axisFactor;
    UNLOCK;
    return this;
}

Engine::Render::Transform *Engine::Render::Transform::Rotate(float yaw, float pitch, float roll)
{
    LOCK;
    Rotation.z += glm::radians(yaw);
    Rotation.x += glm::radians(roll);
    Rotation.y += glm::radians(pitch);
    UNLOCK;
    return this;
}

glm::mat4 Engine::Render::Transform::Construct()
{
    glm::mat4 rotation = glm::yawPitchRoll(Rotation.z, Rotation.y, Rotation.x);
    glm::mat4 translation = glm::translate(glm::mat4(1), Translation);
    glm::mat4 scale = glm::scale(glm::mat4(1), Scalation);
    return translation * rotation * scale;
}