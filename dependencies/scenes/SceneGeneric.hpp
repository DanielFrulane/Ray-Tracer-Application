#ifndef RAY_TRACER_APPLICATION_SCENEGENERIC_HPP
#define RAY_TRACER_APPLICATION_SCENEGENERIC_HPP

#include <vector>
#include "../../include/SDL2/SDL.h"
#include "../RTImage.hpp"
#include "../Camera.hpp"
#include "../materials/MaterialGeneric.hpp"
#include "../materials/MaterialSimple.hpp"
#include "../materials/MaterialRefractiveSimple.hpp"
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

namespace App {
    class SceneGeneric{
    public:
        Camera m_camera;
        std::vector<std::shared_ptr<ObjectGeneric>> m_objectsInScene;
        std::vector<std::shared_ptr<LightGeneric>> m_lightsInScene;
        int m_width, m_height;

    public:
        explicit SceneGeneric(double aspectRatio);

        SceneGeneric();
        virtual ~SceneGeneric();

        bool render(RTImage &outputImage);
        void renderTile(App::TileInformation *tile);
        virtual void generateSceneObjects();

        bool castRay(Ray &castedRay, std::shared_ptr<ObjectGeneric> &closestObject, HitInformation &closestHitInformation);

    private:
        Vector3d renderPixel(int x, int y, int width, int height);
        int convertCoordinatesToLinearIndex(int x, int y, int width, int height);
    };
}

#endif //RAY_TRACER_APPLICATION_SCENEGENERIC_HPP
