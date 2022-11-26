/*!
 * \mainpage	A Code Sample
 * \details		This documentation accompanies the <em> Tin Soldier </em> ray marching render engine.
 *
 * \image		html render.jpg "An n = 2.3 Mandelbulb  rendered by the \em Tin \em Soldier render engine" width = 75%
 *
 * \author		Martijn E. Clemenkowff
 * \date		2022
 */

#include <iostream>
#include <ctime>
#include <string>
// #include "Camera.hpp"
// #include "Components.hpp"
// #include "Scene.hpp"
// #include "RenderCore.hpp"
#include "Render.hpp"

// current date/time based on current system
time_t now = time(0);
    
tm *ltm = localtime(&now);
std::string CurrentTime = std::to_string(1900 + ltm->tm_year) + "_"+
                            std::to_string(1 + ltm->tm_mon) + "_" +
                            std::to_string(ltm->tm_mday) + "_" +
                            std::to_string(ltm->tm_hour) + "_" +
                            std::to_string(ltm->tm_min) + "_" +
                            std::to_string(ltm->tm_sec);

// Camera Orientation

float X = 0.0;
float Y = -4.0;
float Z = 0.0;

float Roll = 0.0;
float Pitch = 0.0;
float Yaw = 90.0;

// Main

int main() {
    
    Camera Camera;
    Scene Scene;
    RenderCore RenderCore;
    Render Render;
    
    // Camera
    
    Camera.SetPosition(X, Y, Z);
    Camera.SetRotation(Roll, Pitch, Yaw);
    
    // Mandelbulb
    
    Mandelbulb* Mandelbulb1 = new Mandelbulb();
    Mandelbulb1->DiffuseColor = glm::vec3(0.73,0.75,0.82);
    Mandelbulb1->Scale = 1;
    Mandelbulb1->Exponent = 2.3;
    Mandelbulb1->MaxIterations = 20;
    Mandelbulb1->EscapeThreshold = 1000;
    
    // Light Sources
    
    DirectionalLight* DirectionalLight1 = new DirectionalLight();
    AmbientLight* AmbientLight1 = new AmbientLight();
    AmbientLight1->Intensity = 0.2;
    DirectionalLight1->Direction = glm::vec3(0.3, 0.5, -1.0);
    
    // Scene
    
    Scene.BackGroundColor = glm::vec3(0., 0., 0.);
    Scene.AddComponent(Mandelbulb1);
    Scene.AddLightSource(DirectionalLight1);
    Scene.AddLightSource(AmbientLight1);

    // Render
    
    Render.SetResolutionScale(1);
    Render.PPM(RenderCore, Camera, Scene, CurrentTime, "./");
    
    return 0;
}
