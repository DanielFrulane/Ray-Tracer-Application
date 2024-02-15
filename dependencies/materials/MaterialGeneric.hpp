#ifndef RAY_TRACER_APPLICATION_MATERIALGENERIC_HPP
#define RAY_TRACER_APPLICATION_MATERIALGENERIC_HPP

#include <memory>
#include "../objects/ObjectGeneric.hpp"
#include "../textures/TextureGeneric.hpp"
#include "../light/LightSource.hpp"
#include "../Ray.hpp"
#include "../../include/Eigen/Eigen"

using namespace Eigen;

namespace App {
    class MaterialGeneric {
    public:
        inline static int m_maximumNumberOfReflections;
        inline static int m_currentNumberOfReflections;
        inline static Vector3d m_ambientColor = {0.0,0.0,0.0};
        inline static double m_ambientColorIntensity = 0.2;
        std::vector<std::shared_ptr<Textures::TextureGeneric>> m_textures;
        bool m_hasTexture = false;
    public:
        MaterialGeneric();
        virtual ~MaterialGeneric();

        virtual Vector3d calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                        const std::vector<std::shared_ptr<LightSource>> &lightList,
                                        const std::shared_ptr<ObjectGeneric> &currentObject,
                                        const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                        const Ray &cameraRay);

        static Vector3d calculateDiffuseColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                              const std::vector<std::shared_ptr<LightSource>> &lightList,
                                              const std::shared_ptr<ObjectGeneric> &currentObject,
                                              const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                              const Vector3d &color);

        Vector3d calculateReflectionColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                          const std::vector<std::shared_ptr<LightSource>> &lightList,
                                          const std::shared_ptr<ObjectGeneric> &currentObject,
                                          const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                          const Ray &incidentRay);

        bool castRay(const Ray &castRay, const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                     const std::shared_ptr<ObjectGeneric> &thisObject,
                     std::shared_ptr<ObjectGeneric> &closestObject,
                     Vector3d &closestIntersectionPoint, Vector3d &closestLocalNormal,
                     Vector3d &closestLocalColor);

        void setTexture(const std::shared_ptr<Textures::TextureGeneric> &newTexture);
    };
}

#endif //RAY_TRACER_APPLICATION_MATERIALGENERIC_HPP
