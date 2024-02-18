#ifndef RAY_TRACER_APPLICATION_SCENE_HPP
#define RAY_TRACER_APPLICATION_SCENE_HPP

#include <vector>
#include "../include/SDL2/SDL.h"
#include "RTImage.hpp"
#include "Camera.hpp"
#include "./materials/MaterialGeneric.hpp"
#include "./materials/MaterialSimple.hpp"
#include "./materials/MaterialRefractiveSimple.hpp"
#include "./objects/ObjectSphere.hpp"
#include "./objects/ObjectPlane.hpp"
#include "./objects/ObjectCylinder.hpp"
#include "./objects/ObjectCone.hpp"
#include "./objects/ObjectCuboid.hpp"
#include "./objects/ObjectComposition.hpp"
#include "./light/LightPoint.hpp"
#include "./textures/TextureFlat.hpp"
#include "./textures/TextureChecker.hpp"
#include "./textures/TextureGradient.hpp"
#include "HitInformation.hpp"
#include <memory>

namespace App {
    class Scene{
    public:
        Camera m_camera;
        std::vector<std::shared_ptr<ObjectGeneric>> m_objectsInScene;
        std::vector<std::shared_ptr<LightSource>> m_lightsInScene;
        int m_width, m_height;

    public:
        explicit Scene(double aspectRatio);

        Scene();

        bool render(RTImage &outputImage);

        bool castRay(Ray &castedRay, std::shared_ptr<ObjectGeneric> &closestObject, HitInformation &closestHitInformation);
    };
}

#endif //RAY_TRACER_APPLICATION_SCENE_HPP
