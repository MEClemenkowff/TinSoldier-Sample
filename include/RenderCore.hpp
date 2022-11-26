#ifndef RenderCore_hpp
#define RenderCore_hpp

//#include <vector>
#include "glm.hpp"
#include <gtx/io.hpp>
//#include <stdio.h>
//#include <iostream>
//#include <fstream>

#include "Components.hpp"
#include "Scene.hpp"

/*!
 * \brief       Class containing all rendering functionality.
 */
class RenderCore
{
    public:

        RenderCore();

        float DistanceThreshold;    ///< Distance to Scene for which Scene is considered to be Hit.
        int MaxIterations;          ///< Number of iterations after which ray marching is terminated.
        float MaxRenderDistance;    ///< Render distance beyond which ray marching is terminated.
        float K;                    ///<
        float MinShadowDepth;       ///<
        float MaxShadowDepth;       ///<
        int AOSteps;                ///<
        float AOStepSize;           ///<

        /*!
         * \brief       Structure containing distance to, and index of, the closest Scene Component.
         */
        struct SDFQuery
        {
            float Distance;         ///< Distance to closest Scene Component.
            size_t Index;           ///< Index of closest Scene Component.
            SDFQuery();
        };

        /*!
         * \brief       Returns the distance to, and index of, the closest Component in Scene
         *              as evaluated from QueryPosition.
         * \param[in]   Scene           The Scene to evaluate.
         * \param[in]   QueryPosition   The position to evaluate the Component SDFs at.
         * \return      SDFQuery        Structure containing distance to, and index of,
         *                              the closest Scene Component.
         */
        SDFQuery EvaluateSceneSDF(Scene Scene, glm::vec3 QueryPosition);

        /*!
         * \brief       Structure containing index of Hit Scene Component, and position at which Hit occurred.
         */
        struct Hit
        {
            float Depth;                    ///< Distance along RayVector at which Hit occurred.
            glm::vec3 Position;             ///< Position of Scene Hit.
            int RaySteps;                   ///< Number of ray steps after which Hit occurred.
            size_t Index;                   ///< Index of Scene Component that was Hit.
            Hit();
        };

        /*!
         * \brief       Returns the Hit of RayVector, originating from RayOrigin, on Scene.
         * \param[in]   Scene           The Scene to evaluate.
         * \param[in]   RayOrigin       The position from which the RayVector originates.
         * \param[in]   RayVector       The vector along which to march.
         * \return      Hit             Structure containing index of Hit Scene Component,
         *                              and position at which Hit occurred.
         */
        Hit RayMarch(glm::vec3 RayOrigin, glm::vec3 RayVector, Scene Scene);

        /*!
         * \brief       Returns an estimate of the surface normal of the Scene Component.
         * \param[in]   SceneComponent  The Scene Component to evaluate.
         * \param[in]   Position        The position for which to estimate the normal.
         * \return      The estimate of the surface normal at Position.
         */
        glm::vec3 EstimateNormals(Component* SceneComponent, glm::vec3 Position);

        /*!
         * \brief       ...
         * \param[in]   Scene           The Scene to evaluate.
         * \param[in]   Hit             ...
         * \param[in]   LightDirection  ...
         * \return      ...
         */
        float SoftShadows(Scene Scene, Hit Hit, glm::vec3 LightDirection);

        /*!
         * \brief       ...
         * \param[in]   Scene           The Scene to evaluate.
         * \param[in]   Hit             ...
         * \param[in]   Normal          ...
         * \return      ...
         */
        float AmbientOcclusion(Scene Scene, Hit Hit, glm::vec3 Normal);

        /*!
         * \brief       ...
         * \param[in]   Scene           The Scene to evaluate.
         * \param[in]   Hit             ...
         * \param[in]   ViewVector      ...
         * \return      ...
         */
        glm::vec3 PhongShading(Scene Scene, Hit Hit, glm::vec3 ViewVector);
};

#endif /* RenderCore_hpp */