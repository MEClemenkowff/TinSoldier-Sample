#include "Camera.hpp"

Camera::Camera()
{
    Position = glm::vec3(0.0, 0.0, 0.0);
    Rotation = glm::vec3(0.0, 0.0, 0.0);
    MovementMode = Cartesian;
    LensMode = Orthographic;
    SensorSize = 0.035;
    OrthographicScale = 6.0f;
    FocalLength = 0.050;
}

void Camera::SetPosition(float X, float Y, float Z)
{
    Position.x = X;
    Position.y = Y;
    Position.z = Z;
}

void Camera::SetRotation(float Roll, float Pitch, float Yaw)
{
    Rotation.x = Roll;
    Rotation.y = Pitch;
    Rotation.z = Yaw;
}

void Camera::SetLensMode(std::string NewMode)
{
    if (NewMode == "Perspective")
    {
        LensMode = Perspective;
    }
    else if (NewMode == "Orthographic")
    {
        LensMode = Orthographic;
    }
}

void Camera::SetFocalLength(float FocalLength)
{
    this->FocalLength = FocalLength/1000;
}

Camera::Ray Camera::CastRayFromPixel(int PixelX, int PixelY, glm::vec2 Resolution)
{

    float Pi = glm::pi<float>();

    Camera::Ray Ray;
    glm::vec3 ForwardVector;
    glm::vec3 Origin;
    glm::vec3 Vector;

    glm::vec3 ViewDirection = Rotation * Pi / 180.f;
    
    // Refactor
    ForwardVector.x = glm::cos(ViewDirection.y) * glm::cos(ViewDirection.z);
    ForwardVector.y = glm::cos(ViewDirection.y) * glm::sin(ViewDirection.z);
    ForwardVector.z = glm::sin(ViewDirection.y);
    
    // Refactor
    glm::vec3 LateralVector = glm::normalize(glm::vec3(-ForwardVector.y, ForwardVector.x, 0.0));
    LateralVector = LateralVector * glm::cos(ViewDirection.x) +
                        glm::cross(ForwardVector, LateralVector) * glm::sin(ViewDirection.x) +
                        ForwardVector * glm::dot(ForwardVector, LateralVector) *
                        (1 - glm::cos(ViewDirection.x));
    
    // Refactor
    glm::vec3 VerticalVector = glm::cross(ForwardVector, LateralVector);
    
    switch (LensMode)
    {
        case Orthographic:
        {
            glm::vec2 OrthographicFactor = glm::vec2(OrthographicScale)*glm::vec2(1,
                                                Resolution.y/Resolution.x);

            Origin = Position + glm::vec3(OrthographicFactor.x * (- PixelX +
                        Resolution.x/2)) * LateralVector/Resolution.x +
                        glm::vec3(OrthographicFactor.y * (- PixelY + Resolution.y/2)) *
                        VerticalVector/Resolution.y;

            Vector = ForwardVector;
            break;
        }
        case Perspective:
        {
            glm::vec2 PerspectiveFactor = glm::vec2(SensorSize)*glm::vec2(1,
                                                Resolution.y/Resolution.x);

            Origin = Position;

            Vector = ForwardVector*glm::vec3(FocalLength) + glm::vec3(PerspectiveFactor.x
                        * (- PixelX + Resolution.x/2)) * LateralVector/Resolution.x +
                        glm::vec3(PerspectiveFactor.y * (- PixelY + Resolution.y/2)) *
                        VerticalVector/Resolution.y;
            Vector = glm::normalize(Vector);
            break;
        }
    }
    
    Ray.Origin = Origin;
    Ray.Vector = Vector;

    return Ray;
}