#ifndef RAY_TRACER_APPLICATION_SCENEGENERIC_HPP
#define RAY_TRACER_APPLICATION_SCENEGENERIC_HPP

#include "../../include/SDL2/SDL.h"
#include "../Camera.hpp"
#include "../materials/MaterialGeneric.hpp"
#include "../materials/MaterialCompleteSimple.hpp"
#include "../objects/ObjectSphere.hpp"
#include "../objects/ObjectPlane.hpp"
#include "../objects/ObjectCylinder.hpp"
#include "../objects/ObjectCone.hpp"
#include "../objects/ObjectCuboid.hpp"
#include "../objects/ObjectComposition.hpp"
#include "../objects/GeometricalTransformation.hpp"
#include "../light/LightPoint.hpp"
#include "../textures/TextureFlat.hpp"
#include "../textures/TextureChecker.hpp"
#include "../textures/TextureGradient.hpp"
#include "../HitInformation.hpp"
#include "../TileInformation.hpp"
#include <memory>
#include <vector>

namespace App {
    class SceneGeneric{
    public:
        Camera m_camera;
        bool m_hasCamera = false;
        int m_width, m_height;
        std::vector<std::shared_ptr<ObjectGeneric>> m_objectsInScene;
        std::vector<std::shared_ptr<LightGeneric>> m_lightsInScene;

        void checkIfHasAllNecessaryComponents() const;

        SceneGeneric();
        virtual ~SceneGeneric();

        virtual void generateSceneObjects();
    };
}

#endif //RAY_TRACER_APPLICATION_SCENEGENERIC_HPP
