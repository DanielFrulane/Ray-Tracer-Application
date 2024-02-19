#ifndef RAY_TRACER_APPLICATION_MATERIALSIMPLE_HPP
#define RAY_TRACER_APPLICATION_MATERIALSIMPLE_HPP

#include "MaterialGeneric.hpp"

namespace App {
    class MaterialSimple : public MaterialGeneric {
    public:
        MaterialSimple();
        virtual ~MaterialSimple() override;

        virtual Vector3d calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                        const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                        const std::shared_ptr<ObjectGeneric> &currentObject,
                                        const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                        const Vector3d &localPointOfIntersection, const Vector2d &uvCoordinates,
                                        const Ray &cameraRay) override;

        // Function to compute specular highlights.
        Vector3d calculateSpecularColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                        const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                        const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                        const App::Ray &cameraRay);

    public:
        Vector3d m_color = {0.0, 0.0, 0.0};
        double m_reflectivity = 0.0;
        double m_shininess = 0.0;
    };
}

#endif //RAY_TRACER_APPLICATION_MATERIALSIMPLE_HPP
