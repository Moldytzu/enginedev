#include <engine/ecs.h>

Engine::ECS::Component::~Component() {}
void Engine::ECS::Component::Start() {}
void Engine::ECS::Component::Update() {}
std::string Engine::ECS::Component::FriendlyName() { return "Generic Component"; }
