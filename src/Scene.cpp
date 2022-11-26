#include "Scene.hpp"

Scene::Scene()
{
    BackGroundColor = glm::vec3(0.0, 0.0, 0.0);
}

void Scene::AddComponent(Component* NewComponent)
{
    Components.push_back(NewComponent);
}

void Scene::AddLightSource(LightSource *NewLightSource)
{
    LightSources.push_back(NewLightSource);
}