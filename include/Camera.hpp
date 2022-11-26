#ifndef Camera_hpp
#define Camera_hpp

#include "glm.hpp"
#include <gtx/io.hpp>

class Camera
{
    private:
        
        enum MovementMode {Cartesian, Orbit};
        MovementMode MovementMode;
        
        enum LensMode {Perspective, Orthographic};
        LensMode LensMode;
        
        float SensorSize;
        
        float OrthographicScale;
        
        float FocalLength;

    public:

        glm::vec3 Position;
        glm::vec3 Rotation;

        Camera();

        void SetPosition(float X, float Y, float Z);
        
        void SetRotation(float Roll, float Pitch, float Yaw);
        
        void SetLensMode(std::string NewMode);
        
        void SetFocalLength(float FocalLength);
        
        void SetSensorSize(float SensorSize);
        
        void SetOrthographicScale(float OrthographicScale);

        /*!
         * \brief       Structure containing ray origin and direction.
         */
        struct Ray
        {
            glm::vec3 Origin;           /// Ray origin point.
            glm::vec3 Vector;           /// Ray direction vector.
        };

        /*!
         * \brief       Returns the origin and direction of a ray cast from the given pixel coordinates.
         * \param[in]   PixelX          Horizontal pixel index to cast ray from.
         * \param[in]   PixelY          Vertical pixel index to cast ray from.
         * \param[in]   Resolution      Resolution of camera sensor.
         * \return      Ray             Structure containing ray origin and direction.
         */
        Ray CastRayFromPixel(int PixelX, int PixelY, glm::vec2 Resolution);
};

#endif /* Camera_hpp */