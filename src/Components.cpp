#include "Components.hpp"

Component::Component()
{
    Position = glm::vec3(0.0, 0.0, 0.0);
    Scale = 1.0;
    DiffuseColor = glm::vec3(0.5, 0.5, 0.5);
    Specularity = 0.45;
    SpecularExponent = 10;
}

Sphere::Sphere()
{
    Radius = 1.0;
}

float Sphere::SDF(glm::vec3 QueryPosition)
{
    return glm::length(QueryPosition - Position) - Radius;
}

Mandelbulb::Mandelbulb()
{
    Exponent = 8;
    MaxIterations = 3;
    EscapeThreshold = 256.0;
}

float Mandelbulb::SDF(glm::vec3 QueryPosition)
{
    QueryPosition = QueryPosition/Scale;
    glm::vec3 w = QueryPosition - glm::vec3(0,0,0);
    float m = glm::dot(w, w);
    float dz = 1.0;

    for (int i = 0; i < MaxIterations; i++)
    {
        dz = Exponent * pow(sqrt(m), Exponent - 1.0 ) * dz + 1.0;

        float r = glm::length(w);
        float b = Exponent*acos(w.y/r);
        float a = Exponent*atan2(w.x, w.z);
        w = QueryPosition + glm::vec3(pow(r, Exponent)) * glm::vec3(sin(b) * sin(a), cos(b), sin(b) * cos(a));

        m = glm::dot(w, w);
        if (m > EscapeThreshold)
        {
            break;
        }
    }

    return 0.25 * log(m) * sqrt(m) / dz * Scale;
}

LightSource::LightSource()
{
    Intensity = 1;
    Position = glm::vec3(0.0, 0.0, 0.0);
    Direction = glm::vec3(0.0, 0.0, 0.0);
    Color = glm::vec3(1.0, 1.0, 1.0);
    Type = Directional;
}

DirectionalLight::DirectionalLight()
{
    Type = Directional;
}

AmbientLight::AmbientLight()
{
    Type = Ambient;
    Intensity = 0.2;
}