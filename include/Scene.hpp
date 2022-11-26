#ifndef Scene_hpp
#define Scene_hpp

#include "glm.hpp"
#include <vector>

#include "Components.hpp"

/*!
 * \brief       Class containing elements of Scene, including Components and LightSources.
*/
class Scene
{
    private:
    
    public:
    
        Scene();
        
        glm::vec3 BackGroundColor;  ///< RGB value of Scene background color.
        
        /*!
         * \brief       Vector containing pointers to all Scene Components.
        */
        std::vector<Component*> Components;
        
        /*!
         * \brief       Vector containing pointers to all Scene LightSources.
        */
        std::vector<LightSource*> LightSources;
        
        /*!
         * \brief       Adds pointer to Component to Scene Components.
         * \param[in]   NewComponent    The Component to add to Scene.
        */
        void AddComponent(Component* NewComponent);
        
        /*!
         * \brief       Adds pointer to LightSource to Scene LightSources.
         * \param[in]   NewLightSource  The LightSource to add to Scene.
        */
        void AddLightSource(LightSource* NewLightSource);

};

#endif /* Scene_hpp */