#ifndef Render_hpp
#define Render_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "glm.hpp"
//#include <gtx/io.hpp>

// #include "Components.hpp"
// #include "Scene.hpp"
#include "Camera.hpp"
#include "RenderCore.hpp"

/*!
 * \todo Move Progress and Clip methods
 * \todo Refactor PPM
*/
class Render
{
    public:

        Render();

        int Clip(int N, int Lower, int Upper);

        void Progress(float Iterator, float Max);

        glm::vec2 Resolution;
        float ResolutionScale;
        
        void SetResolution(float Width, float Height);
        
        void SetResolutionScale(float ResolutionScale);

        std::ofstream PPM(RenderCore RenderCore, Camera Camera, Scene Scene, std::string FileName, std::string Path);

};

#endif /* Render_hpp */