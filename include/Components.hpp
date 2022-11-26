#ifndef Components_hpp
#define Components_hpp

#include "glm.hpp"
#include <gtx/io.hpp>
//#include <iostream>
//#include <stdio.h>

/*!
 * \brief       Parent class of Scene Components.
 * \details     This class includes all characteristics of a Component, e.g. position, scale, rotation and diffuse color.
 */
class Component
{
    public:

        Component();
        
        glm::vec3 Position;         ///< Position of Component.
        float Scale;                ///< Scale of Component.
        glm::vec3 DiffuseColor;     ///< RGB value of diffuse color of Component.
        float Specularity;          ///< Specularity of Component.
        float SpecularExponent;     ///< Specular exponent of Component.

        /*!
         * \brief       Evaluates the Signed Distance Field (SDF) of Component at QueryPosition.
         * \param[in]   QueryPosition   Position at which to evaluate SDF.
         * \return      Shortest distance to Component from QueryPosition.
         */
        virtual float SDF(glm::vec3 QueryPosition)
        {
            return 0;
        }
};

class Sphere: public Component
{
    public:
    
        Sphere();

        float Radius;               ///< Radius of Sphere.
        
        float SDF(glm::vec3 QueryPosition);

};

class Mandelbulb: public Component
{
    public:
        
        Mandelbulb();

        float Exponent;
        
        int MaxIterations;
        
        float EscapeThreshold;
        
        float SDF(glm::vec3 QueryPosition);
};

/*!
 * \brief		Parent class of Scene LightSources.
 * \details		This class includes all characteristics of a LightSource, e.g. position, intensity, type and diffuse color.
 * \todo		Move into own source file.
 */
class LightSource
{
    private:
    
    public:
    
        LightSource();
        
        float Intensity;            ///< Intensity of LightSource.
        glm::vec3 Position;         ///< Position of LightSource.
        glm::vec3 Direction;        ///< Direction of LightSource (not for AmbientLight).
        glm::vec3 Color;            ///< RGB color of LightSource.
        
        enum Type {Directional, Ambient};
        Type Type;                  ///< Type of LightSource.
};

class DirectionalLight: public LightSource
{
    public:
        
        DirectionalLight();

};

class AmbientLight: public LightSource
{
    public:
    
        AmbientLight();

};

#endif /* Components_hpp */