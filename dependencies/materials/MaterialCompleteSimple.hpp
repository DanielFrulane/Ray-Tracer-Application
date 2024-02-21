#ifndef RAY_TRACER_APPLICATION_MATERIALCOMPLETESIMPLE_HPP
#define RAY_TRACER_APPLICATION_MATERIALCOMPLETESIMPLE_HPP

#include "MaterialGeneric.hpp"

namespace App {
    class MaterialCompleteSimple : public MaterialGeneric{
    public:
        double m_reflectivity = 0.0;
        double m_shininess = 0.0;
        double m_translucency = 0.0;
        double m_indexOfRefraction = 0.0;

        MaterialCompleteSimple();
        ~MaterialCompleteSimple();

        virtual Vector3d calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                        const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                        const std::shared_ptr<ObjectGeneric> &currentObject,
                                        const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                        const Vector3d &localPointOfIntersection, const Vector2d &uvCoordinates,
                                        const Ray &cameraRay) override;

        Vector3d calculateSpecularColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                const Ray &cameraRay);

        Vector3d calculateTranslucency(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                const std::shared_ptr<ObjectGeneric> &currentObject,
                                const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                const Ray &cameraRay);
    };
}
#endif //RAY_TRACER_APPLICATION_MATERIALCOMPLETESIMPLE_HPP
