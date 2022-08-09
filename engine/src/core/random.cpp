#include <engine/core.h>
#include <time.h>

int Engine::Core::Random::Int(int low, int high)
{
    return static_cast<int>(rand()) / static_cast<int>(RAND_MAX / (high - low));
}

float Engine::Core::Random::Float(float low, float high)
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (high - low));
}