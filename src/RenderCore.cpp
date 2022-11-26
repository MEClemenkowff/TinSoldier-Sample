#include "RenderCore.hpp"

RenderCore::RenderCore()
{
    DistanceThreshold = 0.0001;
    MaxIterations = 1000;
    MaxRenderDistance = 100.0;
    K = 4.0;
    MinShadowDepth = 0.1;
    MaxShadowDepth = 10;
    AOSteps = 100;
    AOStepSize = 1;
}

RenderCore::SDFQuery::SDFQuery()
{
    Distance = 0;
    Index = 0;
}

RenderCore::SDFQuery RenderCore::EvaluateSceneSDF(Scene Scene, glm::vec3 QueryPosition)
{
    SDFQuery SDFQuery;

    // Loop over all Scene Components. Evaluate SDF and check whether distance is
    // smaller than the previous smallest distance;
    for (size_t i = 0; i != Scene.Components.size(); i++)
        {
            float ComponentDistance = Scene.Components.at(i)->SDF(QueryPosition);
            if (ComponentDistance < SDFQuery.Distance || i == 0)
            {
                SDFQuery.Distance = ComponentDistance;
                SDFQuery.Index = i;
            }
        }
    return SDFQuery;
}

RenderCore::Hit::Hit()
{
    Depth = 0;
    Position = glm::vec3(0.0, 0.0, 0.0);
    RaySteps = 0;
    Index = 0;
}

RenderCore::Hit RenderCore::RayMarch(glm::vec3 RayOrigin, glm::vec3 RayVector, Scene Scene)
{
    Hit Hit;

    // Sphere trace routine
    for (int i = 0; i < MaxIterations; i++ )
    {
        glm::vec3 CurrentPosition = RayOrigin + RayVector * Hit.Depth;
        
        SDFQuery SDFQuery = EvaluateSceneSDF(Scene, CurrentPosition);
        
        // Step shortest distance to Scene forward along ray vector;
        Hit.Depth += SDFQuery.Distance;
        Hit.RaySteps += 1;
        
        // Terminate ray march if;
        if (SDFQuery.Distance <= DistanceThreshold || Hit.Depth > MaxRenderDistance)
        {
            Hit.Position = CurrentPosition;
            Hit.Index = SDFQuery.Index;
            break;
        }
    }

    return Hit;
}

/*!
 * \todo Own DistanceThreshold for Normal estimation.
*/
glm::vec3 RenderCore::EstimateNormals(Component* SceneComponent, glm::vec3 Position)
{
    glm::vec3 Normals;
    Normals.x = SceneComponent->SDF(Position + glm::vec3(DistanceThreshold, 0, 0)) 
                - SceneComponent->SDF(Position - glm::vec3(DistanceThreshold, 0, 0));
    Normals.y = SceneComponent->SDF(Position + glm::vec3(0, DistanceThreshold, 0)) 
                - SceneComponent->SDF(Position - glm::vec3(0, DistanceThreshold, 0));
    Normals.z = SceneComponent->SDF(Position + glm::vec3(0, 0,  DistanceThreshold))
                - SceneComponent->SDF(Position - glm::vec3(0, 0, DistanceThreshold));
    return glm::normalize(Normals);
}

/*!
 * \todo Extract important settings.
 * \todo Write documentation.
*/
float RenderCore::SoftShadows(Scene Scene, Hit Hit, glm::vec3 LightDirection)
{
    float PreviousDistance = 1e20;
    float Depth = MinShadowDepth;
    float ShadowCoefficient = 1.0;

    while (Depth < MaxShadowDepth)
    {
        glm::vec3 CurrentPosition = Hit.Position - LightDirection * Depth;

        SDFQuery SDFQuery = EvaluateSceneSDF(Scene, CurrentPosition);
    
        if (SDFQuery.Distance < 0.001)
        {
            return 0;
        }
        
        float Y = pow(SDFQuery.Distance, 2) / (2.0 * PreviousDistance);
        float D = sqrt(pow(SDFQuery.Distance, 2) - pow(Y, 2));
        ShadowCoefficient = glm::min(ShadowCoefficient, K*D/glm::max(0.0f, Depth-Y));
        PreviousDistance = SDFQuery.Distance;
        Depth += SDFQuery.Distance;
    }
    return ShadowCoefficient;
}

/*!
 * \todo Extract important settings.
 * \todo Write documentation.
*/
float RenderCore::AmbientOcclusion(Scene Scene, Hit Hit, glm::vec3 Normal)
{
    float Sum = 0;
    float MaxSum = 0;
    float Depth = AOStepSize;

    for (int j = 0; j < AOSteps; j ++)
    {
        glm::vec3 CurrentPosition = Hit.Position + Normal * Depth;
    
        SDFQuery SDFQuery = EvaluateSceneSDF(Scene, CurrentPosition);
        
        Sum += 1/pow(2, j) * SDFQuery.Distance;
        MaxSum += 1/pow(2, j) * Depth;
        
        Depth += AOStepSize;
    }
    
    return Sum/MaxSum;
}

glm::vec3 RenderCore::PhongShading(Scene Scene, Hit Hit, glm::vec3 ViewVector)
{
    glm::vec3 RGB = glm::vec3(0.0, 0.0, 0.0);

    if (Hit.Depth < MaxRenderDistance)
    {
        // Component Properties
        
        Component* SceneComponent = Scene.Components.at(Hit.Index);
        glm::vec3 DiffuseColor = SceneComponent->DiffuseColor;
        float Specularity = SceneComponent->Specularity;
        float SpecularExponent = SceneComponent->SpecularExponent;

        glm::vec3 Normal = EstimateNormals(SceneComponent, Hit.Position);
        
        // Loop over all LightSources
        for (size_t i = 0; i != Scene.LightSources.size(); i++)
        {
            LightSource* LightSource = Scene.LightSources.at(i);
            float& Intensity = LightSource->Intensity;
            glm::vec3& LightDirection = LightSource->Direction;
            glm::vec3 Reflection = glm::reflect(-LightDirection, Normal);
            int Type = LightSource->Type;

            switch (Type)
            {
                case 0:
                {
                    // How much received light is reflected?
                    float DiffuseLighting = glm::max(glm::dot(-LightDirection, Normal),
                                            0.0f);
                    float SpecularLighting = Specularity * pow(glm::max(glm::dot(Reflection, ViewVector), 0.0f),
                                             SpecularExponent);
                                                
                    // How much light is received?
                    float Shadow = SoftShadows(Scene, Hit, LightDirection);

                    RGB += Intensity * (DiffuseLighting * DiffuseColor + SpecularLighting) * Shadow;
                    
                    break;
                }

                case 1:
                {
                    float AO = AmbientOcclusion(Scene, Hit, Normal);

                    RGB += Intensity * AO * DiffuseColor;

                    break;
                }
            }
        }
        return RGB;
    }
    
    else
    {
        return Scene.BackGroundColor;
    }
}
