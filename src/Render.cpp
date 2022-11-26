#include "Render.hpp"

Render::Render()
{
    Resolution = glm::vec2(1920.0f, 1080.0f);
    ResolutionScale = 1.0;
}

int Render::Clip(int N, int Lower, int Upper)
{
    return std::max(Lower, std::min(N, Upper));
}

void Render::Progress(float Iterator, float Max)
{
    std::cout.flush();
    std::cout << "Rendering:\t" << (int)(Iterator/Max*100+1) << "\t%\r";
}

void Render::SetResolution(float Width, float Height)
{
    Resolution = glm::vec2(Width, Height);
}

void Render::SetResolutionScale(float ResolutionScale)
{
    Resolution = Resolution * ResolutionScale;
}

/*!
 * \todo Split into write to array and export
*/
std::ofstream Render::PPM(RenderCore RenderCore, Camera Camera, Scene Scene, std::string FileName, std::string Path)
{
    std::ofstream img (Path + FileName + ".ppm");
    img << "P3" << std::endl;
    img << Resolution.x << " " << Resolution.y << std::endl;
    img << "255" << std::endl;
    
    for (int y = 0; y < Resolution.y; y++){
        for (int x = 0; x < Resolution.x; x++){
            
            // Cast ray from camera
            Camera::Ray Ray = Camera.CastRayFromPixel(x, y, Resolution);
            
            // Evaluate SDF to find Hit
            RenderCore::Hit Hit = RenderCore.RayMarch(Ray.Origin, Ray.Vector, Scene);
            
            // Apply shading of pixel based on Hit and context of Scene
            glm::vec3 PhongShading = RenderCore.PhongShading(Scene, Hit, Ray.Vector);
            
            PhongShading.x = Clip(PhongShading.x*255, 0, 255);
            PhongShading.y = Clip(PhongShading.y*255, 0, 255);
            PhongShading.z = Clip(PhongShading.z*255, 0, 255);
            
            img << (int)PhongShading.x << " " << (int)PhongShading.y << " " << (int)PhongShading.z << std::endl;
            
            Progress(Resolution.x*y + x, Resolution.x*Resolution.y);
        }
    }
    return img;
}