#ifndef RAY_TRACER_APPLICATION_SCENE_HPP
#define RAY_TRACER_APPLICATION_SCENE_HPP

#include <vector>
#include "../include/SDL2/SDL.h"
#include "RTImage.hpp"
#include "Camera.hpp"
#include "./objects/ObjectSphere.hpp"
#include "./light/LightPoint.hpp"
#include <memory>

namespace App {
    class Scene{
    private:
        Camera m_camera;
        std::vector<std::shared_ptr<ObjectGeneric>> m_objectsInScene;
        std::vector<std::shared_ptr<LightSource>> m_lightsInScene;

    public:
        explicit Scene(double aspectRatio);

        Scene();

        bool render(RTImage &outputImage);
    };
}

#endif //RAY_TRACER_APPLICATION_SCENE_HPP
