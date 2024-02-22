#ifndef RAY_TRACER_APPLICATION_MATERIALGENERIC_HPP
#define RAY_TRACER_APPLICATION_MATERIALGENERIC_HPP

#include <memory>
#include "../objects/ObjectGeneric.hpp"
#include "../textures/TextureGeneric.hpp"
#include "../light/LightGeneric.hpp"
#include "../Ray.hpp"
#include "../HitInformation.hpp"
#include "../../include/Eigen/Eigen"

using namespace Eigen;

namespace App {
    class MaterialGeneric {
    public:
        inline static int m_maximumNumberOfReflections;
        inline static int m_currentNumberOfReflections;
        std::shared_ptr<Textures::TextureGeneric> m_texture = nullptr;
    public:
        MaterialGeneric();
        virtual ~MaterialGeneric();

        void setTexture(const std::shared_ptr<Textures::TextureGeneric> &newTexture);

        static bool castRay(const Ray &castRay, const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                            const std::shared_ptr<ObjectGeneric> &thisObject,
                            std::shared_ptr<ObjectGeneric> &closestObject,
                            HitInformation &closestHitInformation);

        virtual Vector3d calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                        const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                        const std::shared_ptr<ObjectGeneric> &currentObject,
                                        const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                        const Vector3d &localPointOfIntersection, const Vector2d &uvCoordinates,
                                        const Ray &cameraRay);

        static Vector3d calculateDiffuseColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                              const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                              const std::shared_ptr<ObjectGeneric> &currentObject,
                                              const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                              const Vector3d &color);

        static Vector3d calculateReflectionColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                          const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                          const std::shared_ptr<ObjectGeneric> &currentObject,
                                          const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                          const Ray &incidentRay);
    };
}

#endif //RAY_TRACER_APPLICATION_MATERIALGENERIC_HPP
